#ifndef RECEIVEACTION_H
#define RECEIVEACTION_H

#include "parser/actions/countingaction.h"

class ReceiveAction : public CountingAction
{
public:
    ReceiveAction( int channel, int count )
        : CountingAction( channel, count )
    {
        my_type = ActionTypes::ReceiveAction;
    }

    virtual ByteStatus passNextByte(char)
    {
        if( counter <= 0 )
            return ByteStatus::ActionCompleted;

        counter--;
        return ByteStatus::Receive;
    }

    std::shared_ptr<Action> clone() {
        return std::shared_ptr<Action>(new ReceiveAction(*this));
    }
};

#endif // RECEIVEACTION_H
