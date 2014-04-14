#ifndef ACTION_HEADERS_H
#define ACTION_HEADERS_H

enum class ActionTypes
{
    DropAction, ReceiveAction, Trigger
};

enum class ByteStatus
{
    Drop, Receive, ActionCompleted
};

#endif // ACTION_HEADERS_H
