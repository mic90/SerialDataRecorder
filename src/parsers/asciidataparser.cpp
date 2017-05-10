#include "asciidataparser.h"
#include <QJsonArray>
#include <QDebug>
#include <QsLog.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

QList<QChar> allowedChars{'0','1','2','3','4','5','6','7','8','9','-','+','e','.'};

AsciiDataParser::AsciiDataParser():
    m_buffer("")
{
}

QList<QJsonArray> AsciiDataParser::parse(const QString &data)
{
    m_buffer.append(data);
    if(m_buffer.indexOf('\n') == -1)
    {
        return QList<QJsonArray>();
    }
    QList<QJsonArray> dataList;

    for(QString const& line : m_buffer.split('\n', QString::SkipEmptyParts))
    {
        if(line.isEmpty()) continue;
        QJsonArray jsonArray;
        QString tempValue;

        for(int i=0; i<line.length(); i++)
        {
            if(line.at(i) != ',' && allowedChars.contains(line.at(i)))
            {
                tempValue.append(line.at(i));
            }
            if((line.at(i) == ',' || i == line.length() -1) && !tempValue.isEmpty())
            {
                //delimiter reached, convert to double
                double value = tempValue.toDouble();
                jsonArray.append(value);
                tempValue = "";
            }
        }
        if(!jsonArray.isEmpty())
        {
            dataList.append(jsonArray);
        }
    }

    int lastNewLine = m_buffer.lastIndexOf('\n');
    m_buffer = m_buffer.remove(0, lastNewLine + 1);
    return dataList;
}
