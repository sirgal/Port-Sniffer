#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPair>

#include "interface.h"
#include "portdatasorter.h"

class DataHolder : public QObject
{
    Q_OBJECT

    QList<timestamped_data> unparsed;
    QList<timestamped_data> parsed;

    ParserInterface parser;

public slots:
    void receiveByte( timestamped_data data )
    {
        unparsed.append(data);

        if( parser.nextByte( data.first.first, data.first.second ) ) {
            parsed.append(data);
        }
    }

public:
    DataHolder();

    void reparse()
    {
        parsed.clear();
        parser.reset();

        foreach( timestamped_data data, unparsed ) {
            if( parser.nextByte( data.first.first, data.first.second ) ) {
                parsed.append(data);
            }
        }
    }

    const QList<timestamped_data>& getParsed()
    {
        return parsed;
    }

    const QList<timestamped_data>& getUnparsed()
    {
        return unparsed;
    }

    void setParserProgram( QString parse_line )
    {
        try {
            parser.setParserString( parse_line.toStdString() );
        } catch( ExceptionWithPos &e ) {
            throw;
        }
    }

    QString getPreprocessedString()
    {

    }
};

#endif // DATAHOLDER_H
