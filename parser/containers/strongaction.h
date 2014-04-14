#ifndef STRONGACTION_H
#define STRONGACTION_H

#include <memory>

#include "parser/containers/container.h"
#include "parser/actions/action.h"

template <bool strong = true>
class StrongAction : public Container
{
    std::shared_ptr<Action> action;
public:
    StrongAction( std::shared_ptr<Action> in_action )
        : action(in_action)
    { }

    virtual ByteStatus passByte( int channel, char byte )
    {
        if( action->isDone() )
            return ByteStatus::ActionCompleted;

        if( action->getChannelNumber() == channel )
            return action->passNextByte(byte);
        else
            return strong ? ByteStatus::Drop : ByteStatus::Receive;
    }

    virtual void reset()
    {
        action->reset();
    }

    virtual bool addAction(std::shared_ptr<Action> action) {
        this->action = action;
        return true;
    }
};

#endif // STRONGACTION_H
