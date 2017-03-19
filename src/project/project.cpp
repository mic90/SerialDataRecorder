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

bool Project::load(const QString &path)
{
    setPath(path);
    return load();
}

bool Project::load()
{
    setPath(m_path);
    QLOG_INFO() << "Loading project file" << m_path;
    QFile file(m_path);
    if(!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        QLOG_ERROR() << "Can't open file at" << m_path;
        return false;
    }
    bool ret = fromJson(file.readAll());
    file.close();
    return ret;
}

bool Project::save(const QString &path)
{
    setPath(path);
    return save();
}

bool Project::save()
{
    if(m_path.isEmpty())
    {
        return false;
    }

    QLOG_INFO() << "Saving project to" << m_path << "...";
    QFile f(m_path);
    if(!f.open(QIODevice::WriteOnly))
    {
        QLOG_ERROR() << "Can't open file" << m_path << "for saving";
        return false;
    }
    f.write(toJson());
    f.close();
    return true;
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
    m_serialConfig = serialConfig;
    if(!m_path.isEmpty())
    {
        save();
    }
}

QList<Channel> Project::getChannels() const
{
    return m_channels;
}

void Project::setChannels(const QList<Channel> &variables)
{
    m_channels.clear();
    m_channels.append(variables);
    save();
}

QList<Chart> Project::getCharts() const
{
    return m_charts;
}

void Project::setCharts(const QList<Chart> &charts)
{
    m_charts.clear();
    m_charts.append(charts);
    save();
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
    m_dataParserName = root.value("parser").toString();
    QJsonObject serialConfig = root.value("serial").toObject();
    m_serialConfig.fromJson(serialConfig);
    QJsonArray channels = root.value("channels").toArray();
    for(auto const& jsonChannel : channels)
    {
        Channel newChannel;
        if(newChannel.fromJson(jsonChannel.toObject()))
        {
            m_channels.append(newChannel);
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
    }
    return true;
}

QString Project::getDataParserName() const
{
    return m_dataParserName;
}

void Project::setDataParserName(const QString &dataParserName)
{
    m_dataParserName = dataParserName;
    if(!m_path.isEmpty())
    {
        save();
    }
}
