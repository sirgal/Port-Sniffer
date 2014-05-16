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

#include "dataholder.h"

class DrawData
{
public:
    DrawData();
    ~DrawData();

    void setData(QList<timestamped_data> data );
    void appendData(const timestamped_data data );

    void changeFont(QFont in_font );

    void clear();
    void setNumberBase( int base );

    void setChannelColor( int channel, QColor color );
    void swapChannels( int a, int b );

    void setEnabledChannel(  int channel, bool is_enabled );

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

    QList<timestamped_data> data_list;
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
