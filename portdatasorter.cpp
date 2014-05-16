#include "portdatasorter.h"

PortDataSorter::~PortDataSorter()
{
}

void PortDataSorter::start()
{
    timer.start();
}

void PortDataSorter::byteReceived(char byte)
{
    Channel* channel = qobject_cast<Channel*>( QObject::sender() );

    if( !channel )
        return;

    channel_data_pair pair { channel->getSettings().getNumber(), byte };
    timestamped_data received { pair, timer.nsecsElapsed() };

    emit gotByte( received );
}
