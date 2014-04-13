#ifndef PARAMSTRINGPROCESSOR_H
#define PARAMSTRINGPROCESSOR_H

#include <QObject>
#include <QPair>
#include <QStack>
#include <QList>
#include <QDebug>

/*
 ************ACTIONS:
 ************
 ************RECEIVE:
 * receive given number of bytes on a channel
 * usually used in conjunction with operators (see below)
 * reception does not mean dropping bytes on other channels
 *
 * r[X][Y] - get Y bytes on channel X and move on
 * r[*][Y] - get Y bytes on any channel and move on
 * r[*][*] - receive everything, default command, silently appended to command string
 *
 ************DROP:
 * drops given number of bytes on a channel
 *
 * d[X][Y] - drop Y bytes on channel X
 * d[X][*] - drop everything on channel X
 * d[*][Y] - drop Y bytes, any channel is counted
 * d[*][*] - drop everything (why?)
 * d[X][0] - stop dropping
 *
 ************TRIGGER:
 * waits for specific consecutive bytes, dropping everything else
 * b0b1b2...bN - HEX string (case-insensitive)
 *
 * strong trigger - drop everything on every channel until triggered
 * st[X][b0b1b2...bN]
 * special case:
 * st[X][*] - will drop everything until any activity on channel X
 *
 * weak trigger - drops only on given channel until triggered
 * wt[X][b0b1b2...bN]
 * wt[X][*] is not an error, but is ignored as having no sense
 *
 * can use strings inside second parameter, denoted by '': '<string>'
 * use \ if it is necessary to include symbols ' or ] or \
 * use * in channel number to wait for trigger on any one
 *
 * usage:
 *          wt[1]['\'\]\\'] waits for ']\ on channel 1
 *          st[2]['hey'] - wait for string 'hey' on channel 2
 *          st[2]['hey'0022] - wait for string 'hey', 0x00 and 0x22 on channel 2
 *          st[2][1337'h'4'x'] - you got it
 *          st[3][001122] - wait for 0x00, 0x11, 0x22 consecutive bytes on channel 3
 *          st[3][00112]  - error, 2nd parameter should be even
 *
 ************CYCLE:
 * self-explanatory, can be nested
 *
 * c[<actions>][X] - perform actions X times
 * c[<actions>][*] - perform actions forever
 *
 ************COMMENT:*/
   /* - ignore everything until met */

/************OPERATORS:
 ************
 ************COMMA:
 * perform actions simultaneously (order irrelevant)
 *
 * <drop on X>,<drop on Y> - drop on X and Y simultaneously
 * <drop on X>,<drop on X> - summate drop count for channel X
 *
 * <drops>,<strong trigger>- only strong trigger is effective
 * <drops>,<weak trigger>  - only trigger is effective on its channel
 *
 * <weak trigger>,<strong trigger>   - only strong trigger is effective
 * <weak trigger>,<weak trigger>     - each channel waits for its own trigger
 * <strong trigger>,<strong trigger> - proceed when both triggers activate
 *
 ************COLUMN:
 * <action1>:<action2> - perform action1, THEN action 2
 * actions are individual commands or multiple commands separated with "," for simultaneous usage
 *
 *
*/
/*
class paramStringProcessor : public QObject
{
    Q_OBJECT
public:
    enum Errors {
        noError, invalidChannelNumber, syntaxError, receiveEverything,
        dropEverything, stringTooLong, tooManyNestedLoops, bigNumber,
        noCommentEnd, noCycleEnd, ambiguiousReceiveDrop, unevenBytes
    };

    paramStringProcessor();
    ~paramStringProcessor();

    int getLastError();
    int getLastErrorPosition();
    int setParameterString( QString params );
    void setChannelAmount( int amount );

    friend class paramStringTest;
public slots:
    void processByte( uchar channel, uchar byte );
    void processByte( QPair<uchar, uchar> channel_byte );

signals:
    void byteDropped( uchar channel, uchar byte );
    void byteDropped( QPair<uchar, uchar> channel_byte );

    void byteReceived( uchar channel, uchar byte );
    void byteReceived( QPair<uchar, uchar> channel_byte );

    void gotTrigger( uchar channel );

    void cycleJumpOccured();
private:
    QString internal_param_string;
    QString internal_block_string;
    QString internal_action_string;

    QStack<int> cycle_counters;
    QStack<int> cycle_start_indexes;

    int current_action;
    int current_block;
    int current_block_pos;
    int current_param_pos;

    int last_error;
    int error_position;
    bool in_cycle;
    bool string_valid;

    void stringWalker();

    void receiveProcessor( int channel, int count );
    void dropProcessor( int channel, int count );
    void triggerProcessor( int channel, QByteArray bytes );
    void cycleProcessor( int start, int cycles );

    bool parseTriggerAction();
    bool parseDropReceiveAction();

    QByteArray triggerStringToBytes(QString trigger_string , bool *ok);

    bool validateParamString();
    void getNextCommandBlock();
    void getNextActionBlock();

    bool preprocessString();
    bool removeComments();
};
*/
#endif // PARAMSTRINGPROCESSOR_H
