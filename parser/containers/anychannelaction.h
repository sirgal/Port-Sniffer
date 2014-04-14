#ifndef ANYCHANNELACTION_H
#define ANYCHANNELACTION_H

#include <map>
#include "parser/containers/container.h"

class AnyChannelAction : public Container
{
protected:
    std::shared_ptr<Action> action;
    bool order_matters;
    std::map<int, std::shared_ptr<Action> > channel_to_action;
    bool completed;
public:
    AnyChannelAction( std::shared_ptr<Action> in_action, bool order_matters = false )
        : action(in_action), order_matters(order_matters), completed(false)
    { }

    virtual void reset() {
        action.reset();
    }

    virtual ByteStatus passByte(int channel, char byte)
    {
        if( completed )
            return ByteStatus::ActionCompleted;

        std::shared_ptr<Action> act;

        if( order_matters ) {
            if( channel_to_action.count(channel) == 0 ) {
                std::shared_ptr<Action> copy(action->clone());
                channel_to_action[channel] = copy;
            }

            act = channel_to_action[channel];
        } else {
            act = action;
        }

        ByteStatus result = act->passNextByte(byte);

        if( act->isDone() )
            completed = true;

        return result;
    }

    virtual bool addAction(std::shared_ptr<Action> action) {
        this->action = action;
        return true;
    }
};

#endif // ANYCHANNELACTION_H
