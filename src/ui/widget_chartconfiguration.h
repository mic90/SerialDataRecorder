#ifndef WIDGET_CHARTCONFIGURATION_H
#define WIDGET_CHARTCONFIGURATION_H

#include <QWidget>
#include <QListWidgetItem>
#include <project/chart.h>

namespace Ui {
class WidgetChartConfiguration;
}

class WidgetChartConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetChartConfiguration(QWidget *parent = 0);
    ~WidgetChartConfiguration();

    void setChart(Chart const& chart);
    void setChannels(QList<Channel> const& channels);

signals:
    void titleChanged(QString const& title);
    void xAxisChanged(QString const& axis);
    void yAxisChanged(QString const& axis);
    void yAxisAutorangeChanged(bool value);
    void yAxisMinChanged(int value);
    void yAxisMaxChanged(int value);
    void samplesCountChanged(int value);
    void channelsChanged(QList<int> const& channels);

private slots:
    void on_autorange_toggled(bool checked);
    void on_title_editingFinished();
    void on_xAxis_editingFinished();
    void on_yAxis_editingFinished();
    void on_samplesCount_valueChanged(int arg1);
    void on_yAxisMin_valueChanged(int arg1);
    void on_yAxisMax_valueChanged(int arg1);

    void on_channels_clicked(const QModelIndex &index);

private:
    Ui::WidgetChartConfiguration *ui;
    QList<QListWidgetItem*> m_channelItems;
};

#endif // WIDGET_CHARTCONFIGURATION_H
