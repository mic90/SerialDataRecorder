#include "asciidataparser.h"
#include <QJsonArray>
#include <QDebug>
#include <QsLog.h>

AsciiDataParser::AsciiDataParser():
    m_buffer("")
{

}

QList<QJsonArray> AsciiDataParser::parse(const QString &data)
{
    m_buffer.append(data);
    if(!m_buffer.endsWith('\n'))
    {
        return QList<QJsonArray>();
    }
    QList<QJsonArray> dataList;
    for(auto const& line : m_buffer.split('\n'))
    {
        if(line.isEmpty()) continue;
        QJsonArray jsonArray;
        for(auto const& value : line.split(','))
        {
            jsonArray.append(value.toDouble());
        }
        dataList.append(jsonArray);
    }

    m_buffer.clear();
    return dataList;
}
