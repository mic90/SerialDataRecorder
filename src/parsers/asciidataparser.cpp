#include "asciidataparser.h"
#include <QJsonArray>
#include <QDebug>

const QString DELIM = "\n\r";

AsciiDataParser::AsciiDataParser():
    m_buffer(""),
    m_counter(0)
{

}

QList<QJsonArray> AsciiDataParser::parse(const QString &data)
{
    m_buffer.append(data);
    if(!m_buffer.endsWith('\n'))
    {
        return QList<QJsonArray>();
    }
    QJsonArray jsonArray;
    for(auto const& value : m_buffer.split(','))
    {
        jsonArray.append(value.toDouble());
    }
    m_buffer.clear();
    return QList<QJsonArray>() << jsonArray;
}
