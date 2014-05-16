#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include <memory>

#include "algorithms/parser.h"

class ParserInterface
{
    std::vector<std::shared_ptr<Container>> containers;
    CyclicContainer sniffer;
    Parser parser;
    std::string preprocessed;

public:
    ParserInterface() { }

    void setParserString( std::string parser_string )
    {
        try {
            preprocessed = parser.preprocessString( parser_string );
            containers = parser.parseString( preprocessed );
        } catch( ExceptionWithPos &e ) {
            throw;
        }

        sniffer.setContainers( containers );
        sniffer.setCycles( 1 );
    }

    bool nextByte( int channel, char byte )
    {
        switch( sniffer.passByte( channel, byte ) ) {
        case ByteStatus::Receive:
            return true;
        case ByteStatus::ActionCompleted:
            return true; // reached the end of program
        case ByteStatus::Drop:
            return false;
        default:
            return false;
        }
    }

    void reset()
    {
        sniffer.reset();
    }

    std::string getPreprocessed()
    {
        return preprocessed;
    }
};

#endif // INTERFACE_H

