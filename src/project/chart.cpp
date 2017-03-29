#include "chart.h"
#include <QJsonObject>
#include <QJsonArray>

Chart::Chart(): Chart(QString())
{

}

Chart::Chart(const QString &name):
    m_name(name),
    m_xAxis("x"),
    m_yAxis("y"),
    m_yAxisAutorange(false),
    m_yAxisMin(0.0),
    m_yAxisMax(1.0),
    m_xAxisRange(10000),
    m_minimumHeight(150)
{

}

Chart::Chart(const Chart &other)
{
    m_name = other.m_name;
    m_xAxis = other.m_xAxis;
    m_yAxis = other.m_yAxis;
    m_yAxisAutorange = other.m_yAxisAutorange;
    m_yAxisMin = other.m_yAxisMin;
    m_yAxisMax = other.m_yAxisMax;
    m_xAxisRange = other.m_xAxisRange;
    m_minimumHeight = other.m_minimumHeight;
    m_channels.append(other.m_channels);
}

bool Chart::operator ==(const Chart &other)
{
    return m_name == other.m_name &&
            m_xAxis == other.m_xAxis &&
            m_yAxis == other.m_yAxis &&
            m_yAxisAutorange == other.m_yAxisAutorange &&
            m_yAxisMin == other.m_yAxisMin &&
            m_yAxisMax == other.m_yAxisMax &&
            m_xAxisRange == other.m_xAxisRange &&
            m_minimumHeight == other.m_minimumHeight &&
            m_channels == other.m_channels;
}

QJsonObject Chart::toJson() const
{
    QJsonArray channels;
    for(auto channel : m_channels)
    {
        channels.append(channel);
    }
    QJsonObject chartObj
    {
        {"name", m_name},
        {"xAxis", m_xAxis},
        {"yAxis", m_yAxis},
        {"yAxisAutorange", m_yAxisAutorange},
        {"yAxisMin", m_yAxisMin},
        {"yAxisMax", m_yAxisMax},
        {"samples", m_xAxisRange},
        {"minimumHeight", m_minimumHeight},
        {"channels", channels}
    };
    return chartObj;
}

bool Chart::fromJson(const QJsonObject &obj)
{
    if(!obj.contains("name")
            || !obj.contains("xAxis")
            || !obj.contains("yAxis") || !obj.contains("yAxisAutorange")
            || !obj.contains("yAxisMin") || !obj.contains("yAxisMax")
            || !obj.contains("samples") || !obj.contains("minimumHeight")
            || !obj.contains("channels"))
    {
        return false;
    }
    m_name = obj.value("name").toString();
    m_xAxis = obj.value("xAxis").toString();
    m_yAxis = obj.value("yAxis").toString();
    m_yAxisAutorange = obj.value("yAxisAutorange").toBool();
    m_yAxisMin = obj.value("yAxisMin").toDouble();
    m_yAxisMax = obj.value("yAxisMax").toDouble();
    m_xAxisRange = obj.value("samples").toInt();
    m_minimumHeight = obj.value("minimumHeight").toInt();
    auto channels = obj.value("channels").toArray();
    for(auto const& channel : channels)
    {
        m_channels.append(channel.toInt());
    }
    return true;
}

QString Chart::name() const
{
    return m_name;
}

void Chart::setName(const QString &name)
{
    m_name = name;
}

QString Chart::xAxis() const
{
    return m_xAxis;
}

void Chart::setXAxis(const QString &xAxis)
{
    m_xAxis = xAxis;
}

QString Chart::yAxis() const
{
    return m_yAxis;
}

void Chart::setYAxis(const QString &yAxis)
{
    m_yAxis = yAxis;
}

void Chart::setChannels(const QList<int> &channels)
{
    m_channels = channels;
}

QList<int> Chart::channels() const
{
    return m_channels;
}

int Chart::minimumHeight() const
{
    return m_minimumHeight;
}

void Chart::setMinimumHeight(int minimumHeight)
{
    m_minimumHeight = minimumHeight;
}

int Chart::xAxisRange() const
{
    return m_xAxisRange;
}

void Chart::setXAxisRange(int xAxisRange)
{
    m_xAxisRange = xAxisRange;
}

double Chart::yAxisMin() const
{
    return m_yAxisMin;
}

void Chart::setYAxisMin(double yAxisMin)
{
    m_yAxisMin = yAxisMin;
}

double Chart::yAxisMax() const
{
    return m_yAxisMax;
}

void Chart::setYAxisMax(double yAxisMax)
{
    m_yAxisMax = yAxisMax;
}

bool Chart::yAxisAutorange() const
{
    return m_yAxisAutorange;
}

void Chart::setYAxisAutorange(bool yAxisAutorange)
{
    m_yAxisAutorange = yAxisAutorange;
}
