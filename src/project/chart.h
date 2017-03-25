#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <project/channel.h>

class Chart
{
public:
    Chart();
    explicit Chart(QString const& name);
    Chart(Chart const& other);

    ~Chart() = default;

    QJsonObject toJson() const;
    bool fromJson(QJsonObject const& obj);

    QString name() const;
    void setName(const QString &name);
    QString xAxis() const;
    void setXAxis(const QString &xAxis);
    QString yAxis() const;
    void setYAxis(const QString &yAxis);
    bool yAxisAutorange() const;
    void setYAxisAutorange(bool yAxisAutorange);
    double yAxisMin() const;
    void setYAxisMin(double yAxisMin);
    double yAxisMax() const;
    void setYAxisMax(double yAxisMax);
    int xAxisRange() const;
    void setXAxisRange(int xAxisRange);
    int minimumHeight() const;
    void setMinimumHeight(int minimumHeight);

    void setChannels(const QList<int> &channels);
    QList<int> channels() const;

private:
    QString m_name;
    QString m_xAxis;
    QString m_yAxis;
    bool m_yAxisAutorange;
    double m_yAxisMin;
    double m_yAxisMax;
    int m_xAxisRange;
    int m_minimumHeight;
    QList<int> m_channels;
};

#endif // CHART_H
