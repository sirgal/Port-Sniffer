#include "portdatasorter.h"

PortDataSorter::~PortDataSorter()
{
}

void PortDataSorter::byteReceived(char byte)
{
    Channel* channel = qobject_cast<Channel*>( QObject::sender() );
    QPair<int, char> pair;
    QPair<QPair<int, char>, long long int> received;

    pair.first = channel->getSettings().getNumber();
    pair.second = byte;

    received.first = pair;
    received.second = timer.nsecsElapsed();

    emit gotByte( received );
}
