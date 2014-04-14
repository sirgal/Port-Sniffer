#ifndef DROPACTION_H
#define DROPACTION_H

#include "parser/actions/countingaction.h"

class DropAction : public CountingAction
{
public:
    DropAction( int channel, int count )
        : CountingAction( channel, count )
    {
        my_type = ActionTypes::DropAction;
    }

    virtual ByteStatus passNextByte(char)
    {
        if( counter <= 0 )
            return ByteStatus::ActionCompleted;

        counter--;
        return ByteStatus::Drop;
    }

    std::shared_ptr<Action> clone() {
        return std::shared_ptr<Action>(new DropAction(*this));
    }
};

#endif // DROPACTION_H
