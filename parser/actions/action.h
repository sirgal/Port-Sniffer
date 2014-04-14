#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <memory>
#include "parser/actions/action_enums.h"

class Action
{
    int channel_number;
protected:
    ActionTypes my_type;

    Action( int channel )
        : channel_number(channel)
    { }
public:
    virtual ~Action() { }
    virtual bool isDone() = 0;
    virtual ByteStatus passNextByte(char) = 0;
    virtual void reset() = 0;

    int getChannelNumber()
    {
        return channel_number;
    }

    ActionTypes getType()
    {
        return my_type;
    }

    void setChannel( int channel )
    {
        channel_number = channel;
    }

    virtual std::shared_ptr<Action> clone() = 0;
};

#endif // ACTION_H
