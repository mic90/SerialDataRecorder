#include "mysensorsdataparser.h"
#include <QJsonObject>
#include <QsLog.h>

QJsonObject MySensorsDataParser::parse(const QString &data)
{
    m_tempData += data;
    if(m_tempData.endsWith("\n")) {
        for(auto line : m_tempData.split("\n")) {
            if(line.isEmpty()) {
                continue;
            }
            QLOG_DEBUG() << "MYS:" << line;
            QStringList lineParsed = line.split(';');
            if(lineParsed.size() < 5) {
                continue;
            }
            int nodeId = lineParsed.at(0).toInt();
            int sensorId = lineParsed.at(1).toInt();
            int messageType = lineParsed.at(2).toInt();
            int messageSubType = lineParsed.at(4).toInt();
            QString value = lineParsed.last();
            if(messageType == 0) {
                //received presentation message
                QLOG_INFO() << "New sensor detected, node:" << nodeId << ", sensor:" << sensorId << ", type:" << messageSubType;
                continue;
            }
            if(messageType == 1) {
                //received sensor read
                QJsonObject readObject;
                readObject.insert("id", QString::number(nodeId) + "/" + QString::number(sensorId));
                readObject.insert("value", value);
                return readObject;
            }
        }
        m_tempData.clear();
    }
    return QJsonObject();
}

MySensorsDataParser::~MySensorsDataParser()
{

}
