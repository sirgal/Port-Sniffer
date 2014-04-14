#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>
#include "parser/actions/action.h"
#include "parser/actions/action_enums.h"

class Container
{
public:
    virtual ~Container() { }
    virtual ByteStatus passByte( int channel, char byte ) = 0;
    virtual void reset() = 0;
    //dummy method, for the ease of testing only
    virtual bool addAction( std::shared_ptr<Action> ) { return false; }
};

#endif // CONTAINER_H
