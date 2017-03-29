#include "project.h"
#include <QFile>
#include <QsLog.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

Project::Project(const QString &path, QObject *parent) : QObject(parent),
    m_path(path),
    m_name(""),
    m_serialConfig(),
    m_dataParserName("")
{

}

QString Project::path() const
{
    return m_path;
}

void Project::setPath(const QString &path)
{
    m_path = path;
    QLOG_DEBUG() << "Path changed to" << m_path;
    QFileInfo fInfo(path);
    m_name = fInfo.baseName();
    emit pathChanged(m_path);
}

QString Project::name() const
{
    return m_name;
}

SerialPortConfig Project::serialConfig() const
{
    return m_serialConfig;
}

void Project::setSerialConfig(const SerialPortConfig &serialConfig)
{
    if(m_serialConfig == serialConfig)
    {
        return;
    }
    m_serialConfig = serialConfig;
    emit projectChanged();
}

QList<Channel> Project::getChannels() const
{
    return m_channels;
}

void Project::setChannels(const QList<Channel> &channels)
{
    if(m_channels == channels)
    {
        return;
    }
    m_channels.clear();
    m_channels.append(channels);
    emit projectChanged();
}

QList<Chart> Project::getCharts() const
{
    return m_charts;
}

void Project::setCharts(const QList<Chart> &charts)
{
    if(m_charts == charts)
    {
        return;
    }
    m_charts.clear();
    m_charts.append(charts);
    emit projectChanged();
}

QByteArray Project::toJson()
{
    QJsonDocument document;
    QJsonObject root
    {
        {"name", m_name},
        {"parser", m_dataParserName},
        {"serial", m_serialConfig.toJson()}
    };
    QJsonArray channels;
    for(auto const& channel : m_channels)
    {
        channels.append(channel.toJson());
    }
    root.insert("channels", channels);
    QJsonArray charts;
    for(auto const& chart : m_charts)
    {
        charts.append(chart.toJson());
    }
    root.insert("charts", charts);
    document.setObject(root);
    return document.toJson();
}

bool Project::fromJson(const QByteArray &data)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject root = document.object();
    if(root.isEmpty())
    {
        QLOG_ERROR() << "Root object is empty";
        return false;
    }
    m_name = root.value("name").toString();
    m_dataParserName = root.value("parser").toString();
    QJsonObject serialConfig = root.value("serial").toObject();
    if(!m_serialConfig.fromJson(serialConfig)) {
        return false;
    }
    QJsonArray channels = root.value("channels").toArray();
    for(auto const& jsonChannel : channels)
    {
        Channel newChannel;
        if(newChannel.fromJson(jsonChannel.toObject()))
        {
            m_channels.append(newChannel);
        }
        else
        {
            return false;
        }
    }
    QJsonArray charts = root.value("charts").toArray();
    for(auto const& jsonChart: charts)
    {
        Chart newChart;
        if(newChart.fromJson(jsonChart.toObject()))
        {
            m_charts.append(newChart);
        }
        else
        {
            return false;
        }
    }
    return true;
}

QString Project::getDataParserName() const
{
    return m_dataParserName;
}

void Project::setDataParserName(const QString &dataParserName)
{
    if(m_dataParserName == dataParserName)
    {
        return;
    }
    m_dataParserName = dataParserName;
    emit projectChanged();
}
