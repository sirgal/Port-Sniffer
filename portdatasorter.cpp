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

    QPair<int, char> pair { channel->getSettings().getNumber(), byte };
    QPair<QPair<int, char>, long long int> received { pair, timer.nsecsElapsed() };

    emit gotByte( received );
}
