#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <algorithm>
#include <locale>

#include "parser/algorithms/parserexceptions.h"

#include "parser/actions/dropaction.h"
#include "parser/actions/receiveaction.h"
#include "parser/actions/triggeraction.h"
#include "parser/actions/action_enums.h"

#include "parser/containers/container.h"
#include "parser/containers/actioncontainer.h"
#include "parser/containers/anychannelaction.h"
#include "parser/containers/cycliccontainer.h"

const unsigned int channel_count = 16;

class Parser
{
    std::string input_string;
    std::vector<std::shared_ptr<Container>> containers;
    std::map<std::string, ActionTypes> name_to_type
    {
        {"d", ActionTypes::DropAction},
        {"r", ActionTypes::ReceiveAction},
        {"t", ActionTypes::Trigger}
    };

public:
    Parser(std::string string)
    {
        try {
            input_string = preprocessString( string );
        } catch( ... ) {
            throw;
        }
    }
    Parser() { }

    std::string charToCode( char ch )
    {
        char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
        std::string result;
        result += hex_chars[(ch & 0xF0) >> 4];
        result += hex_chars[(ch & 0x0F) >> 0];
        return result;
    }

    std::string stringToCodes( std::string input )
    {
        std::string result;

        //remove escapes from escaped characters
        std::size_t escape_pos = 0;
        while( ( escape_pos = input.find('\\', escape_pos) ) != std::string::npos ) {
            input.erase( escape_pos, 1 );
            escape_pos++;
        }

        for( char ch : input ) {
            result += charToCode(ch);
        }

        return result;
    }

    std::vector<char> codesToBytes( std::string input ) {
        if( input.size() % 2 )
            throw InvalidCodeFormat(0);

        std::vector<char> byte_vector;

        for( unsigned int i = 0; i < input.size(); i += 2 ) {
            std::string byte;
            byte += input[i];
            byte += input[i+1];

            try {
                byte_vector.push_back( std::stoi( byte, 0, 16 ) );
            } catch(...) {
                throw InvalidCodeFormat(i);
            }
        }

        return byte_vector;
    }

    std::string preprocessString( std::string input )
    {
        std::string result = input;

        //remove comments
        std::size_t comment_start = 0, comment_end = 0;

        while( ( comment_start = result.find("/*") ) != std::string::npos ) {
            if( ( comment_end = result.find("*/") ) != std::string::npos ) {
                //the /*/ construction is considered a valid comment
                if( comment_end <= comment_start )
                    throw InvalidComment( comment_end );
                else
                    result.erase( comment_start, comment_end - comment_start + 2 );

            } else {
                throw InfiniteComment( comment_start );
            }
        }

        //prepare trigger strings
        std::size_t trigstr_start = 0, trigstr_end = 0;

        while( ( trigstr_start = result.find('"', trigstr_start) ) != std::string::npos ) {
            trigstr_end = trigstr_start + 1;

            while( ( trigstr_end = result.find('"', trigstr_end) ) != std::string::npos ) {
                if( trigstr_end > 0 && result[trigstr_end-1] == '\\' ) {
                    trigstr_end++;
                    continue;
                }

                std::string trigstr = result.substr( trigstr_start + 1, trigstr_end - trigstr_start - 1 );
                result.replace( trigstr_start, trigstr_end - trigstr_start + 1, stringToCodes(trigstr) );

                break;
            }
            //this will help with infinite cycles
            trigstr_start++;
        }

        //check if everything is ok
        std::size_t error_pos = result.find('"');

        if( error_pos != std::string::npos )
            throw SyntaxError('"', error_pos);

        result.erase( std::remove( result.begin(), result.end(), ' ' ), result.end());
        result.erase( std::remove( result.begin(), result.end(), '\n' ), result.end());

        return result;
    }

    std::shared_ptr<Action> buildAction( std::string action_string )
    {
        //split action type, channel string and parameter
        //tried to (and should) use regexp here. BUT IT'S NOT SUPPORTED BY MY COMPILER.
        int channel_num_start = action_string.find('[') + 1;
        int channel_num_end   = action_string.find(']', channel_num_start);
        int parameter_start   = action_string.find('[', channel_num_end) + 1;
        int parameter_end     = action_string.size() - 1;

        std::string action_type   = action_string.substr( 0, channel_num_start - 1 );
        std::string channel_str   = action_string.substr( channel_num_start, channel_num_end - channel_num_start );
        std::string parameter_str = action_string.substr( parameter_start, parameter_end - parameter_start );

        int channel_number = 0;

        if( channel_str.find('*') != std::string::npos ) {
            channel_number = -1;
        } else {
            try {
                channel_number = std::stoi(channel_str);
            } catch(...) {
                throw NonNumericParam(channel_num_start);
            }
        }

        if( name_to_type.count(action_type) == 0 )
            throw UnknownActionType(0);

        ActionTypes type = name_to_type[ action_type ];

        int param = 0;

        switch (type) {
        case ActionTypes::DropAction:

            try {
                param = std::stoi(parameter_str);
            } catch(...) {
                throw NonNumericParam(parameter_start);
            }

            return std::shared_ptr<Action>( new DropAction(channel_number, param) );

        case ActionTypes::ReceiveAction:

            try {
                param = std::stoi(parameter_str);
            } catch(...) {
                throw NonNumericParam(parameter_start);
            }

            return std::shared_ptr<Action>( new ReceiveAction(channel_number, param) );

        case ActionTypes::Trigger:
            return std::shared_ptr<Action>( new TriggerAction(channel_number, codesToBytes(parameter_str) ) );

        default:
            throw UnknownActionType(0);
        }
    }

    std::shared_ptr<Container> buildContainer( std::string container_string )
    {
        std::vector<std::shared_ptr<Action> > actions;
        std::vector<std::string> action_strings;
        action_strings.push_back("");
        bool strong_actions = false;
        bool any_channel_action = false;

        for( char ch : container_string ) {
            if( ch == ',' ){
                action_strings.push_back("");
                continue;
            }

            action_strings[action_strings.size()-1] += ch;
        }

        int action_count = action_strings.size();
        bool first_action = true;

        int accumulated_length = 0;
        for( std::string string : action_strings ) {
            //set initial strongness
            if( first_action ) {
                strong_actions = ( string[0] == 's' );
                first_action = false;
            } else {
                //check if action is considered "strong"
                if( (string[0] == 's') ^ strong_actions )
                    throw StrongViolation(accumulated_length);
            }
            //remove 's' so it won't interfere with buildAction
            if( strong_actions )
                string.erase( 0, 1 );

            std::shared_ptr<Action> action;

            try {
                action = buildAction( string );
            } catch( ExceptionWithPos &e ) {
                e.position += accumulated_length;
                throw;
            }

            //any-channel-action should be the only one in the container
            if( action->getChannelNumber() == -1  ) {
                any_channel_action = true;

                if( action_count > 1 )
                    throw AnyChannelViolation(accumulated_length);
            } else {
                any_channel_action = false;
            }

            actions.push_back( action );

            accumulated_length += string.length();
        }

        if( any_channel_action ) {
            //order matters only if it's a trigger
            //obviously, strongness doesn't matter when the action works on every channel
            bool order_matters = ( actions[0]->getType() == ActionTypes::Trigger );
            return std::shared_ptr<Container>( new AnyChannelAction( actions[0], order_matters ) );
        } else {
            std::shared_ptr<Container> action_cont( new ActionContainer<channel_count>(strong_actions) );

            for( auto action : actions ) {
                if( !action_cont->addAction( action ) )
                    throw IncompatibleActions(accumulated_length);
            }

            return action_cont;
        }
    }

    std::vector<std::shared_ptr<Container> > parseString( std::string to_parse )
    {
        std::vector<std::shared_ptr<Container> > containers;
        std::string container_string;
        bool in_cycle = false;
        int bracket_count = 0;

        //yeah, it's a horrible state machine
        //but it's a parser, so it's fine
        for( unsigned int i = 0; i < to_parse.size(); i++ ) {
            char ch = to_parse[i];

            if( in_cycle ) {
                switch( ch ) {
                case '[':
                    bracket_count++;
                    break;
                case ']':
                    bracket_count--;
                    break;
                default:
                    break;
                }

                if( bracket_count == 0 ) {
                    //extract cycle number string
                    i += 2;
                    int cycle_end = to_parse.find(']', i) + 1;
                    std::string cycles_string = to_parse.substr( i, cycle_end - i - 1 );
                    int cycles;
                    try {
                        cycles = std::stoi( cycles_string );
                    } catch(...) {
                        throw NonNumericParam(i);
                    }

                    std::shared_ptr<CyclicContainer> cyclic_cont(
                                new CyclicContainer( cycles, parseString(container_string) ) );

                    containers.push_back( cyclic_cont );

                    container_string.clear();
                    in_cycle = false;

                    i = cycle_end;  //jump over : if it is there

                    if( i < to_parse.size() ){
                        if( to_parse[i] != ':' )
                            //mixing cycle with anything is not allowed
                            throw CycleMixing(i);
                    }

                    continue;
                }

                container_string += ch;

            } else {

                switch( ch ) {
                case 'c':
                    in_cycle = true;
                    //jump over starting bracket
                    i++;
                    bracket_count = 1;
                    break;
                case ':':
                    try {
                        containers.push_back( buildContainer( container_string ) );
                    } catch( ExceptionWithPos &e ) {
                        e.position += i - container_string.length();
                        throw;
                    }
                    container_string.clear();
                    break;
                default:
                    container_string += ch;
                    break;
                }
            }
        }

        if( !container_string.empty() ) {
            try {
                containers.push_back( buildContainer( container_string ) );
            } catch( ExceptionWithPos &e ) {
                e.position += to_parse.length() - container_string.length();
                throw;
            }
        }

        return containers;
    }
};

#endif // PARSER_H
