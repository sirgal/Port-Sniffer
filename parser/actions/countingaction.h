#ifndef COUNTINGACTION_H
#define COUNTINGACTION_H

#include "parser/actions/action.h"

class CountingAction : public Action
{
protected:
    int initial_counter;
    int counter;

    CountingAction(int channel, int in_count )
        : Action(channel), initial_counter(in_count), counter(in_count)
    { }
public:
    //true: merge succeeded; false: merge failed - incompatible types;
    bool mergeWith(CountingAction *action)
    {
        //if actions are of the same type and are on the same channel - merge
        if( action->getType() == my_type && action->getChannelNumber() == getChannelNumber() ){
            counter += action->counter;
            initial_counter += action->initial_counter;
            return true;
        } else {
            return false;
        }
    }

    void reset()
    {
        counter = initial_counter;
    }

    bool isDone()
    {
        return counter == 0;
    }
};

#endif // COUNTINGACTION_H
