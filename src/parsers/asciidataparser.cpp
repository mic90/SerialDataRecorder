#include "asciidataparser.h"
#include <QJsonArray>

AsciiDataParser::AsciiDataParser():
    m_buffer("")
{

}

QJsonArray AsciiDataParser::parse(const QString &data)
{
    m_buffer.append(data);
    if(!m_buffer.endsWith("\n\r"))
    {
        return QJsonArray();
    }
    QJsonArray jsonArray;
    auto trimmed = m_buffer.trimmed();
    for(auto const& value : trimmed.split(","))
    {
        jsonArray.append(value.toDouble());
    }
    m_buffer.clear();
    return jsonArray;
}
