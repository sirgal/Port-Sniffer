#ifndef DRAWDATA_H
#define DRAWDATA_H

#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QBitArray>
#include <QPair>
#include <QDebug>
#include <QFontMetrics>
#include <QTime>
#include <chrono>
#include <random>

class DrawData
{
public:
    DrawData();
    ~DrawData();

    void setDrawData(QVarLengthArray<QPair<uchar, uchar>, 1024> data );

    void appendData( uchar channel, uchar data );
    void appendData( QPair<uchar, uchar> data );

    void changeFont(QFont in_font );

    void clear();
    void setNumberBase( int base );

    void setChannelColor( uchar channel, QColor color );
    void swapChannels( uchar a, uchar b );

    void enableChannel(  int channel );
    void disableChannel( int channel );

    bool createImage(int data_start, QPixmap *result );
    int dataAvailable();
    int maxChannels();

    int channelsAvailable();
    void setChannelCount( int new_count );

private:
    int horizontal_position;
    int channel_vertical_offset;
    int data_horizontal_offset;
    const int channel_count;
    int channels_available;
    int last_channel;
    int number_base;
    bool init;

    QRect font_rectangle;
    QFont font;

    QVarLengthArray<QPair<uchar, uchar>, 1024> data_list;
    QList<int> channel_map;
    QColor *channel_colors;
    bool   *disabled_channels;

    QPixmap  *picture;
    QPainter *painter;

    bool drawDataToPixmap(int picture_position, int array_index, QPixmap *pixmap);

    void niceRandomColor( QColor &color )
    {
        color = QColor( qrand()%100 + 155, //r
                        qrand()%100 + 155, //g
                        qrand()%100 + 155  //b
                       );
    }
};

#endif // DRAWDATA_H
