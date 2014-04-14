#ifndef CYCLECONTAINER_H
#define CYCLECONTAINER_H

#include <vector>
#include <memory>

#include "parser/containers/container.h"

class CyclicContainer : public Container
{
protected:
    int cycles_left;
    int cycles;
    unsigned int current_container;
    std::vector<std::shared_ptr<Container>> containers;

    void resetContainers()
    {
        current_container = 0;

        for( auto container : containers )
            container->reset();
    }
public:
    CyclicContainer() : cycles_left(0), cycles(0), current_container(0) { }

    CyclicContainer( int cycles, std::vector<std::shared_ptr<Container>> containers )
        : cycles_left(cycles), cycles(cycles), current_container(0), containers(containers)
    { }

    void setContainers( std::vector<std::shared_ptr<Container>> containers ) {
        this->containers = containers;
    }

    ByteStatus passByte( int channel, char byte )
    {
        if( cycles_left == 0 )
            return ByteStatus::ActionCompleted;

        std::shared_ptr<Container> container = containers[current_container];

        //actually, not forever, will return
        for(;/*_*/;) {
            auto result = container->passByte(channel, byte);

            if( result == ByteStatus::ActionCompleted )
                current_container++;
            else
                return result;

            if( current_container >= containers.size() ){
                // infinite cycle if it's negative
                if( cycles_left > 0 )
                    cycles_left--;

                if( cycles_left == 0 )
                    return ByteStatus::ActionCompleted;
                else
                    resetContainers();
            }

            container = containers[current_container];
        }
    }

    void reset()
    {
        cycles_left = cycles;

        resetContainers();
    }

    void setCycles( int cycles )
    {
        cycles = cycles;
        cycles_left = cycles;
    }
};

#endif // CYCLECONTAINER_H
