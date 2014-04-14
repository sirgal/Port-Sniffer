#ifndef TRIGGERACTION_H
#define TRIGGERACTION_H

#include <vector>
#include "parser/actions/action.h"
#include "parser/algorithms/streamkmp.h"

class TriggerAction : public Action
{
    StreamKMP algorithm;
public:
    TriggerAction( int channel, std::vector<char> in_trigger )
        : Action(channel), algorithm(in_trigger)
    {
        my_type = ActionTypes::Trigger;
    }

    virtual ByteStatus passNextByte(char byte)
    {
        if( isDone() )
            return ByteStatus::ActionCompleted;

        algorithm.nextChar(byte);

        return ByteStatus::Drop;
    }

    virtual void reset()
    {
        algorithm.reset();
    }

    virtual bool isDone()
    {
        return algorithm.isDone();
    }

    std::shared_ptr<Action> clone() {
        return std::shared_ptr<Action>(new TriggerAction(*this));
    }
};

#endif // TRIGGERACTION_H
