#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include <map>
#include <bitset>
#include <memory>
#include "parser/containers/container.h"
#include "parser/actions/countingaction.h"

template <unsigned int channel_count>
class ActionContainer : public Container
{
protected:
    std::map<int, std::shared_ptr<Action> > actions;
    //used to avoid overhead of referring to map during action addition
    std::bitset<channel_count> used_channels;
    std::bitset<channel_count> unfinished_actions;
    bool strong;
public:
    ActionContainer( bool strong = false ) : strong(strong) { }

    //true: action added; false: incompatible action fault;
    bool addAction( std::shared_ptr<Action> to_add )
    {
        int channel = to_add->getChannelNumber();

        //if channel is used already
        if( used_channels[channel] ) {
            //check if both actions are of mergeable type
            CountingAction *existing_act = dynamic_cast<CountingAction*>(actions.at(channel).get());
            CountingAction *new_act      = dynamic_cast<CountingAction*>(to_add.get());

            //actions are of non-counting type and cannot be merged
            if( existing_act == nullptr || new_act == nullptr )
                return false;

            //try merging both actions
            //if it fails - actions are of different types
            if( !existing_act->mergeWith(new_act) )
                return false;
            else //we succeeded in merging actions, done
                return true;

        } else { //new channel is used, add action to it
            used_channels[channel] = true;
            unfinished_actions[channel] = true;
            actions[channel] = to_add;
            return true;
        }
    }

    //processes one byte, passing it to an appropriate action
    ByteStatus passByte( int channel, char byte )
    {
        //we're done
        if( unfinished_actions.none() )
            return ByteStatus::ActionCompleted;

        //receive or drop all bytes not checked by this container
        if( !used_channels.test(channel) )
            return strong ? ByteStatus::Drop : ByteStatus::Receive;

        auto byte_status = actions.at(channel)->passNextByte(byte);

        if( actions.at(channel)->isDone() ) {
            unfinished_actions[channel] = false;
        }

        if( byte_status == ByteStatus::ActionCompleted ) {
            if( unfinished_actions.none() )
                return ByteStatus::ActionCompleted;
            else
                return ByteStatus::Receive;
        } else {
            return byte_status;
        }
    }

    //resets state of every action
    virtual void reset()
    {
        for( auto action : actions )
            action.second->reset();

        unfinished_actions = used_channels;
    }
};

#endif // ACTIONCONTAINER_H
