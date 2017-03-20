#include "asciidataparser.h"
#include <QJsonArray>

AsciiDataParser::AsciiDataParser():
    m_buffer("")
{

}

void AsciiDataParser::parse(const QString &data)
{
    for(auto const& msg : data.split("\n\r"))
    {
        if(msg.isEmpty())
        {
            continue;
        }

        QJsonArray jsonArray;
        auto trimmed = msg.trimmed();
        for(auto const& value : trimmed.split(","))
        {
            jsonArray.append(value.toDouble());
        }
        emit dataReady(jsonArray);
    }
}
