#include "drawdata.h"

DrawData::DrawData() : channel_count(16), number_base(10)
{
    init = true;

    painter             = new QPainter();
    channel_colors      = new QColor[channel_count];
    disabled_channels   = new bool[channel_count];

    for( int i = 0; i < channel_count; i++ ) {
        niceRandomColor( channel_colors[i] );
        disabled_channels[i] = false;
        channel_map.append(i);
    }

    changeFont( QFont( "consolas", 8 ) );

    init = false;
}

DrawData::~DrawData()
{
    delete [] disabled_channels;
    delete [] channel_colors;
    delete painter;
}

int DrawData::channelsAvailable()
{
    return channels_available;
}

void DrawData::setChannelCount( int new_count )
{
    channels_available = new_count;
}

void DrawData::setNumberBase(int base)
{
    if( base <= 1 )
        number_base = 10;
    else
        number_base = base;
}

void DrawData::changeFont( QFont in_font )
{
    font = in_font;

    font_rectangle = QRect( QFontMetrics(font).boundingRect( "00" ) );
}

void DrawData::setDrawData( QVarLengthArray<QPair<uchar, uchar>, 1024 > data )
{
    data_list = QVarLengthArray<QPair<uchar, uchar>, 1024 >( data );
}

void DrawData::appendData( uchar channel, uchar data )
{
    appendData( QPair<uchar, uchar>( channel, data ) );
}

void DrawData::appendData( QPair<uchar, uchar> data )
{
    data_list.append( data );
}

void DrawData::clear()
{
    data_list.clear();
    channel_map.clear();
    for( int i = 0; i < channel_count; i++ ) {
        channel_map.append(i);
    }
}

// false - failed to set color (channel out of range)
void DrawData::setChannelColor( uchar channel, QColor color )
{
    if( channel < channel_count )
        channel_colors[channel] = color;
}

void DrawData::swapChannels( uchar a, uchar b )
{
    channel_map.move( a, b );
}

void DrawData::setEnabledChannel(int channel, bool is_enabled)
{
    if( channel < channel_count )
        disabled_channels[channel] = is_enabled;
}

int DrawData::maxChannels()
{
    return channel_count;
}

// false - failed to draw
bool DrawData::drawDataToPixmap( int picture_position, int array_index, QPixmap *pixmap )
{
    uchar channel = data_list.at( array_index ).first;
    uchar data    = data_list.at( array_index ).second;

    // map accordingly to current channel swaps
    channel = channel_map.indexOf( channel );

    // channel out of bounds, no color available - error
    if( channel >= channel_count || channel >= channels_available )
        return false;
    if( disabled_channels[channel] )
        return false;

    // get channel color
    QColor text_color( channel_colors[channel] );

    // calculate channel vertical position
    // and text horizontal position
    int vertical_pos = channel * channel_vertical_offset + font_rectangle.height();
    int horizontal_position = picture_position * data_horizontal_offset;

    // start drawing
    painter->begin( pixmap );
    painter->setFont( font );
    painter->setPen( text_color );

    // draw vertical line if channel changed
    if( last_channel != channel ) {
        painter->fillRect( horizontal_position,         // x start
                           0,                           // y start
                           font_rectangle.width()/2,   // line width
                           pixmap->height(),            // line height
                           text_color
                          );
        last_channel = channel;
    }

    // draw text box
    QPoint rectangle_pos( horizontal_position, vertical_pos );
    QRect  text_box( font_rectangle.translated( rectangle_pos ).adjusted(
                        0,
                        0,
                        font_rectangle.width() /2, //make it 2 times bigger than text box
                        font_rectangle.height()/2
                       )
                    );

    painter->fillRect( text_box, text_color );

    // make text readable
    // QColor inverse_text_color( 255 - text_color.red(), 255 - text_color.green(), 255 - text_color.blue() );
    painter->setPen( Qt::black );

    // draw text
    QString data_to_draw = QString( "%1" ).arg( (int) data, 2, 16, QChar('0') ).toUpper();
    // place it in the middle of text box
    QPoint text_pos( text_box.left()   + text_box.width() /4,
                     text_box.bottom() - text_box.height()/4
                    );
    painter->drawText( text_pos, data_to_draw );

    // draw byte number (vertically)
    painter->setPen( Qt::black );
    painter->translate( horizontal_position + data_horizontal_offset, pixmap->height() );
    painter->rotate( 270 );
    painter->drawText( 0, 0, QString::number( array_index, number_base ) );

    painter->end();

    return true;
}

bool DrawData::createImage( int data_start, QPixmap *result )
{
    result->fill();
    last_channel = -1;

    data_horizontal_offset  = font_rectangle.width() * 1.5;
    channel_vertical_offset = font_rectangle.height() * 1.5;

    int bytes_fitting = ceil( result->width() / (double) data_horizontal_offset );

    if( channel_vertical_offset * ( channel_count + 1 ) >= result->height() ) {
        channel_vertical_offset = result->height() / (channel_count + 4);
    }

    for( int bytes_drawn = 0, index = data_start;
         bytes_drawn < bytes_fitting && index < data_list.size();
         index++
        ) {
        if( drawDataToPixmap( bytes_drawn, index, result ) )
            bytes_drawn++;
    }

    return true;
}

int DrawData::dataAvailable()
{
    return data_list.size();
}
