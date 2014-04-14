#ifndef TESTCASES_H
#define TESTCASES_H

#include <string>
#include <iostream>
#include <memory>

#include "parser/actions/dropaction.h"
#include "parser/actions/receiveaction.h"
#include "parser/actions/triggeraction.h"

#include "parser/containers/actioncontainer.h"
#include "parser/containers/anychannelaction.h"
#include "parser/containers/cycliccontainer.h"
#include "parser/containers/strongaction.h"

#include "parser/algorithms/parser.h"
#include "parser/algorithms/streamkmp.h"

class ActionTests
{
public:
    void testAll() {
        testCountingAction();
        testDrop();
        testReceive();
        testTrigger();
    }

    void testCountingAction()
    {
        std::cout << "Counting action test begins..." << std::endl;

        DropAction action_1(1, 5);
        DropAction action_2(1, 5);
        ReceiveAction action_3(1, 5);

        CountingAction *count_1 = dynamic_cast<CountingAction*>(&action_1);
        CountingAction *count_2 = dynamic_cast<CountingAction*>(&action_2);
        CountingAction *count_3 = dynamic_cast<CountingAction*>(&action_3);

        if( count_1 == nullptr || count_2 == nullptr || count_3 == nullptr ) {
            std::cout << "Casting failed" << std::endl;
            return;
        }

        if( count_1->mergeWith(count_3) )
            std::cout << "Merged incompatible types" << std::endl;

        if( !count_1->mergeWith(count_2) )
            std::cout << "Merging failed" << std::endl;

        for( int i = 0; i < 10; i++ ) {
            if( action_1.passNextByte(i) != ByteStatus::Drop ) {
                std::cout << "Merging is incorrect" << std::endl;
                return;
            }
        }

        if( action_1.passNextByte(0) != ByteStatus::ActionCompleted )
            std::cout << "Merging is incorrect" << std::endl;
    }
    void testDrop()
    {
        std::cout << "Drop action test begins..." << std::endl;

        DropAction action(1, 5);

        if( action.getChannelNumber() != 1 )
            std::cout << "Channel number is bad" << std::endl;

        if( action.getType() != ActionTypes::DropAction )
            std::cout << "Type is bad" << std::endl;

        for( int i = 0; i < 5; i++ ) {
            if( action.passNextByte(i) != ByteStatus::Drop ) {
                std::cout << "Wrong byte status" << std::endl;
                break;
            }
        }

        if( action.passNextByte(0) != ByteStatus::ActionCompleted )
            std::cout << "Action completition failed" << std::endl;
    }
    void testReceive()
    {
        std::cout << "Receive action test begins..." << std::endl;

        ReceiveAction action(1, 5);

        if( action.getChannelNumber() != 1 )
            std::cout << "Channel number is bad" << std::endl;

        if( action.getType() != ActionTypes::ReceiveAction )
            std::cout << "Type is bad" << std::endl;

        for( int i = 0; i < 5; i++ ) {
            if( action.passNextByte(i) != ByteStatus::Receive ) {
                std::cout << "Wrong byte status" << std::endl;
                break;
            }
        }

        if( action.passNextByte(0) != ByteStatus::ActionCompleted )
            std::cout << "Action completition failed" << std::endl;
    }

    void testTrigger()
    {
        std::cout << "Trigger action test begins..." << std::endl;

        std::string str = "ABCDABD";
        std::vector<char> substring(str.begin(), str.end());
        str = "ABC ABCDAB ABCDABCDABDE";
        std::vector<char> test_case(str.begin(), str.end());

        TriggerAction action(1, substring);

        if( action.getChannelNumber() != 1 )
            std::cout << "Channel number is bad" << std::endl;

        if( action.getType() != ActionTypes::Trigger )
            std::cout << "Type is bad" << std::endl;

        int counter = 0;
        for( char ch : test_case ) {
            ByteStatus result = action.passNextByte(ch);
            if( result == ByteStatus::Receive ) {
                std::cout << "Byte received when it should be dropped" << std::endl;
            } else if( result == ByteStatus::ActionCompleted ) {
                std::cout << "Trigger done at: " << counter
                          << ", expected 22" << std::endl;
                return;
            }
            counter++;
        }

        std::cout << "Trigger didn't trigger. TEST FAILED." << std::endl;
    }


};

class ContainerTests
{
public:
    void testAll() {
        testActionContainer();
        testAnyChannelActionContainer();
        testCyclicContainer();
        testStrongAction();
    }

    void testActionContainer()
    {
        const int channel_count = 3;
        std::cout << "Action container test begins..." << std::endl;

        ActionContainer<channel_count> weak_container(false);
        ActionContainer<channel_count> strong_container(true);

        std::shared_ptr<ReceiveAction> receive(new ReceiveAction(1, 10));
        std::shared_ptr<DropAction> drop(new DropAction(1, 10));

        for( int z = 0; z < 2; z++ ){
            Container *current_one;
            if( z )
                current_one = dynamic_cast<Container*>( &strong_container );
            else
                current_one = dynamic_cast<Container*>( &weak_container );

            if( z )
                std::cout << "Strong container test" << std::endl;
            else
                std::cout << "Weak container test" << std::endl;

            if( current_one == nullptr ) {
                std::cout << "Casting failed" << std::endl;
                return;
            }

            if( !current_one->addAction(receive) )
                std::cout << "Action adding failed" << std::endl;

            if( current_one->addAction(drop) )
                std::cout << "Incompatible action rejection failed" << std::endl;

            if( !current_one->addAction(receive) )
                std::cout << "Action merging failed" << std::endl;

            drop->setChannel(2);

            if( !current_one->addAction(drop) )
                std::cout << "Another action adding failed" << std::endl;

            for( int i = 0; i < 10; i++ ) {
                if( current_one->passByte(0, i) != ( z ? ByteStatus::Drop : ByteStatus::Receive ) ) {
                    std::cout << "Other channer receive/drop fail" << std::endl;
                    break;
                }
            }

            for( int i = 0; i < 10; i++ ) {
                if( current_one->passByte(2, i) != ByteStatus::Drop ) {
                    std::cout << "Drop action fail" << std::endl;
                    break;
                }
            }

            if( current_one->passByte(2, 0) != ByteStatus::Receive )
                std::cout << "Finished action reception fail" << std::endl;

            //20 because we merged two receive actions
            for( int i = 0; i < 20; i++ ) {
                if( current_one->passByte(1, i) != ByteStatus::Receive ) {
                    std::cout << "Receive action fail" << std::endl;
                    break;
                }
            }

            if( current_one->passByte(1, 0) != ByteStatus::ActionCompleted )
                std::cout << "Container finish fail" << std::endl;

            drop = std::shared_ptr<DropAction>(new DropAction(1, 10));
            receive = std::shared_ptr<ReceiveAction>(new ReceiveAction(1, 10));
        }
    }

    void testAnyChannelActionContainer()
    {
        std::cout << "Any channel container test begins..." << std::endl;

        std::shared_ptr<DropAction> action(new DropAction(1, 10));
        AnyChannelAction container(action);

        for( int i = 0; i < 10; i++ ) {
            if( container.passByte(i, i) != ByteStatus::Drop ) {
                std::cout << "Drop failed" << std::endl;
                break;
            }
        }

        if( container.passByte(0, 1) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail" << std::endl;
    }

    void testCyclicContainer()
    {
        std::cout << "Cyclic container test begins..." << std::endl;

        std::shared_ptr<DropAction> action(new DropAction(1, 10));

        std::shared_ptr<ActionContainer<2>> internal_cont(new ActionContainer<2>);
        internal_cont->addAction(action);

        std::vector<std::shared_ptr<Container>> cont_vect;
        cont_vect.push_back(internal_cont);

        CyclicContainer cont(10, cont_vect);

        for( int z = 0; z < 2; z++ ) {
            for( int i = 0; i < 10; i++ ) {
                //each second byte will go to the 0th channel (empty)
                for( int j = 0; j < 10*2; j++ ) {
                    ByteStatus expected = (j % 2) == 0 ? ByteStatus::Receive : ByteStatus::Drop;

                    if( cont.passByte( j % 2, 0 ) != expected ) {
                        std::cout << "Cycling failed at iter: " << i << " byte: " << j << std::endl;
                    }
                }
            }

            if( cont.passByte( 1, 0 ) != ByteStatus::ActionCompleted )
                std::cout << "Container finish fail" << std::endl;

            cont.reset();
        }
    }

    void testStrongAction()
    {
        std::cout << "Strong/weak action container test begins..." << std::endl;

        std::shared_ptr<ReceiveAction> sr_action(new ReceiveAction(1,10));
        std::shared_ptr<DropAction> wd_action(new DropAction(1,10));

        std::unique_ptr<StrongAction<true>> strong_action(new StrongAction<true>(sr_action));
        std::unique_ptr<StrongAction<false>> weak_action(new StrongAction<false>(wd_action));

        for( int z = 0; z < 2; z++ ){
            Container *current_one;
            if( z )
                current_one = dynamic_cast<Container*>( strong_action.get() );
            else
                current_one = dynamic_cast<Container*>( weak_action.get() );

            if( z )
                std::cout << "Strong action container test" << std::endl;
            else
                std::cout << "Weak action container test" << std::endl;

            if( current_one == nullptr ) {
                std::cout << "Casting failed" << std::endl;
                return;
            }

            ByteStatus action_resp = z ? ByteStatus::Receive : ByteStatus::Drop;
            ByteStatus empty_channel = z ? ByteStatus::Drop : ByteStatus::Receive;

            for( int i = 0; i < 20; i++ ) {
                ByteStatus expected = ( i % 2 ) == 0 ? empty_channel : action_resp;
                if( current_one->passByte( i%2, 0 ) != expected ){
                    std::cout << "Weak\\strong test failed" << std::endl;
                    break;
                }
            }

            if( current_one->passByte( 0, 0 ) != ByteStatus::ActionCompleted )
                std::cout << "Channel finish failed" << std::endl;
        }
    }
};

class AlgoTests
{
public:
    void testAll() {
        testKMP();
        testParser();
    }

    void testParser()
    {
        std::cout << "Parser test begins..." << std::endl;
        Parser parser("d[11][15]");

        std::cout << "Build action test begins..." << std::endl;
        std::shared_ptr<Action> act = parser.buildAction("d[11][15]");

        if( act->getChannelNumber() != 11 )
            std::cout << "Channel number fail" << std::endl;

        for( int i = 0; i < 15; i++ ) {
            if( act->passNextByte(0) != ByteStatus::Drop ) {
                std::cout << "Drop fail" << std::endl;
                break;
            }
        }

        if( act->passNextByte(0) != ByteStatus::ActionCompleted )
            std::cout << "Action finish fail" << std::endl;

        std::cout << "Build normal container test begins..." << std::endl;
        std::shared_ptr<Container> cont = parser.buildContainer("r[1][15],d[2][10]");

        for( int i = 0; i < 15; i++ ) {
            if( cont->passByte(1, 0) != ByteStatus::Receive ) {
                std::cout << "Reception fail" << std::endl;
                break;
            }
        }

        for( int i = 0; i < 10; i++ ) {
            if( cont->passByte(2, 0) != ByteStatus::Drop ) {
                std::cout << "Drop fail" << std::endl;
                break;
            }
        }

        if( cont->passByte(3, 0) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail" << std::endl;

        try {
            cont = parser.buildContainer("r[1][15],d[1][10]");
            std::cout << "Incompatible actions exception fail" << std::endl;
        } catch(...) {
            //...
        }

        try {
            cont = parser.buildContainer("r[1][15],sd[2][10]");
            std::cout << "Strong violation exception fail" << std::endl;
        } catch(...) {
            //...
        }

        try {
            cont = parser.buildContainer("r[1][15],r[*][10]");
            std::cout << "Any channel violation exception fail" << std::endl;
        } catch(...) {
            //...
        }

        try {
            cont = parser.buildContainer("TOPLEL[1][15],r[5][10]");
            std::cout << "Unknown action exception fail" << std::endl;
        } catch(...) {
            //...
        }

        std::cout << "Build any channel container test begins..." << std::endl;
        cont = parser.buildContainer("r[*][15]");

        for( int i = 0; i < 15; i++ ) {
            if( cont->passByte(i, 0) != ByteStatus::Receive ) {
                std::cout << "Reception fail" << std::endl;
                break;
            }
        }

        if( cont->passByte(3, 0) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail" << std::endl;

        std::cout << "Build trigger container test begins..." << std::endl;
        cont = parser.buildContainer("t[0][000102],t[1][030405]");

        for( int i = 0; i <3 /*you*/; i++ ){
            if( cont->passByte(0, i) != ByteStatus::Drop ) {
                std::cout << "Drop fail 1" << std::endl;
                break;
            }
        }

        if( cont->passByte(0, 1) != ByteStatus::Receive )
            std::cout << "Trigger open fail 1" << std::endl;

        for( int i = 3; i < 6; i++ ){
            if( cont->passByte(1, i) != ByteStatus::Drop ) {
                std::cout << "Drop fail 2" << std::endl;
                break;
            }
        }

        if( cont->passByte(3, 0) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail" << std::endl;

        std::cout << "String preprocessing test starts..." << std::endl;

        std::string result = parser.preprocessString(
                    "/*receive 1 byte on 1 channel*/r[1][1],t[10]/*this is a trigger*/[\"hello\"00\"ano\\\\ther\\\"s\"99\"LEL\"]"
                    );

        if( result != "r[1][1],t[10][68656C6C6F00616E6F5C746865722273994C454C]" )
            std::cout << "String preprocessing failed" << std::endl;

        try {
            parser.preprocessString("/*r[1][2]");
            std::cout << "Neverending comment exception fail" << std::endl;
        } catch(...) {
            //...
        }

        try {
            parser.preprocessString("*/r[1][2]/*");
            std::cout << "Incorrect comment exception fail" << std::endl;
        } catch(...) {
            //...
        }

        std::cout << "Cyclic container test starts..." << std::endl;

        std::vector<std::shared_ptr<Container>> conts = parser.parseString("c[d[1][10]:c[r[1][1]][10]][3]");
        //3 iterations
        for( int z = 0; z < 3; z++ ) {
            //drop 10 normally
            for( int i = 0; i < 10; i++ ) {
                if( conts[0]->passByte(1, 1) != ByteStatus::Drop ) {
                    std::cout << "Drop fail" << std::endl;
                    break;
                }
            }
            //receive 10 in an internal cycle
            for( int i = 0; i < 10; i++ ) {
                if( conts[0]->passByte(1, 1) != ByteStatus::Receive ) {
                    std::cout << "Drop fail" << std::endl;
                    break;
                }
            }
        }

        if( conts[0]->passByte(1, 1) != ByteStatus::ActionCompleted )
            std::cout << "Cyclic container finish fail" << std::endl;

        std::cout << "Final exam starts..." << std::endl;

        std::string final_exam =
"d[2][15],r[1][15]:/*drop 15 on 2, receive 10 on 1*/\
d[1][10],t[2][\"hello\"]:/*drop 10 on 1, wait for hello on 2*/\
sr[1][5],sd[2][10]:/*receive 5 on 1, drop 10 on 2, dropping everything on other channels*/\
c[/*in cycle*/\
st[1][\"strong\"]:/*wait for strong on 1, dropping everything other*/\
c[\
c[r[1][1]][10]:\
c[d[2][1]][10]\
][10]/*and, 10 times receive 10 on 1, drop 10 on 2*/\
][5]/*5 times*/";

        conts = parser.parseString( parser.preprocessString( final_exam ));

        for( int i = 0; i < 15; i++ ) {
            if( conts[0]->passByte(2, 1) != ByteStatus::Drop ) {
                std::cout << "Drop fail 1" << std::endl;
                break;
            }
            if( conts[0]->passByte(3, 1) != ByteStatus::Receive ) {
                std::cout << "Receive (other channel) fail 1" << std::endl;
                break;
            }
            if( conts[0]->passByte(1, 1) != ByteStatus::Receive ) {
                std::cout << "Receive fail 1" << std::endl;
                break;
            }
        }

        if( conts[0]->passByte(1, 1) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail 1" << std::endl;

        const char hello[] = "hello";

        for( int i = 0; i < 5; i++ ) {
            if( conts[1]->passByte(1, 1) != ByteStatus::Drop ) {
                std::cout << "Drop fail 2" << std::endl;
                break;
            }
            if( conts[1]->passByte(2, hello[i]) != ByteStatus::Drop ) {
                std::cout << "Trigger drop fail 1" << std::endl;
                break;
            }
            if( conts[1]->passByte(3, 1) != ByteStatus::Receive ) {
                std::cout << "Receive (other channel) fail 2" << std::endl;
                break;
            }
        }
        for( int i = 0; i < 5; i++ ) {
            if( conts[1]->passByte(2, hello[i]) != ByteStatus::Receive ) {
                std::cout << "Trigger receive fail 1" << std::endl;
                break;
            }
            if( conts[1]->passByte(3, 1) != ByteStatus::Receive ) {
                std::cout << "Receive (other channel) fail 2" << std::endl;
                break;
            }
            if( conts[1]->passByte(1, 1) != ByteStatus::Drop ) {
                std::cout << "Drop fail 2" << std::endl;
                break;
            }
        }

        if( conts[1]->passByte(1, 1) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail 2" << std::endl;

        for( int i = 0; i < 10; i++ ) {
            if( conts[2]->passByte(1, 1) != ByteStatus::Receive ) {
                std::cout << "Strong receive fail 1" << std::endl;
                break;
            }
            if( conts[2]->passByte(3, 1) != ByteStatus::Drop ) {
                std::cout << "Drop (other channel) fail 2" << std::endl;
                break;
            }
            if( conts[2]->passByte(2, 1) != ByteStatus::Drop ) {
                std::cout << "Strong drop fail 1" << std::endl;
                break;
            }
        }

        if( conts[2]->passByte(1, 1) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail 3" << std::endl;

        const char strong[] = "strong";

        for( int z = 0; z < 5; z++ ) {
            for( int i = 0; i < 6; i++ ) {
                if( conts[3]->passByte(1, strong[i]) != ByteStatus::Drop ) {
                    std::cout << "Trigger drop fail 2" << std::endl;
                    break;
                }
            }

            for( int i = 0; i < 10; i++ ) {
                for( int j = 0; j < 10; j++ ) {
                    if( conts[3]->passByte(3, 1) != ByteStatus::Receive ) {
                        std::cout << "Receive (other channel) fail 3" << std::endl;
                        break;
                    }
                    if( conts[3]->passByte(1, 1) != ByteStatus::Receive ) {
                        std::cout << "Receive fail 3" << std::endl;
                        break;
                    }
                }
                for( int j = 0; j < 10; j++ ) {
                    if( conts[3]->passByte(1, 1) != ByteStatus::Receive ) {
                        std::cout << "Receive (other channel) fail 3" << std::endl;
                        break;
                    }
                    if( conts[3]->passByte(2, 1) != ByteStatus::Drop ) {
                        std::cout << "Drop fail 3" << std::endl;
                        break;
                    }
                }
            }
        }

        if( conts[3]->passByte(1, 1) != ByteStatus::ActionCompleted )
            std::cout << "Container finish fail 4" << std::endl;
    }

    void testKMP()
    {
        std::cout << "Knuth-Morris-Pratt test begins..." << std::endl;

        std::string str = "ABCDABD";
        std::vector<char> substring(str.begin(), str.end());
        str = "ABC ABCDAB ABCDABCDABDE";
        std::vector<char> test_case(str.begin(), str.end());

        StreamKMP testee = StreamKMP(substring);

        int counter = 0;
        for( char ch : test_case ) {
            if( testee.nextChar(ch) ) {
                std::cout << "Substring end found at: " << counter
                          << ", expected 21" << std::endl;
                return;
            }
            counter++;
        }

        std::cout << "Substring was not found. TEST FAILED." << std::endl;
    }
};

#endif // TESTCASES_H
