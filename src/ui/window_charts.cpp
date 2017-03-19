#include "ui/window_charts.h"
#include "ui_window_charts.h"
#include <QDebug>

ChartsListModel::ChartsListModel(QObject *parent): QAbstractListModel(parent)
{

}

int ChartsListModel::rowCount(const QModelIndex&) const
{
    return m_charts.size();
}

QVariant ChartsListModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return m_charts.at(index.row()).name();
    }
    if(role == Qt::DecorationRole)
    {
        return QIcon(":/icons/charts/chart_line_link.png");
    }

    return QVariant();
}

QList<Chart> ChartsListModel::charts() const
{
    return m_charts;
}

void ChartsListModel::setCharts(const QList<Chart> &charts)
{
    beginResetModel();
    m_charts = charts;
    endResetModel();
}

void ChartsListModel::addChart()
{
    beginInsertRows(QModelIndex(), m_charts.size(), m_charts.size());
    Chart newChart(QString("Chart%1").arg(m_charts.size()));
    m_charts.append(newChart);
    endInsertRows();
}

void ChartsListModel::removeChart(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_charts.removeAt(index.row());
    endRemoveRows();
}

void ChartsListModel::setTitle(const QModelIndex &index, const QString &title)
{
    m_charts[index.row()].setName(title);
    emit dataChanged(index, index);
}

Chart &ChartsListModel::getChart(const QModelIndex &index)
{
    return m_charts[index.row()];
}

//------------------------------------------------------------------------------

WindowCharts::WindowCharts(QList<Chart> charts, QList<Channel> channels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowCharts)
{
    ui->setupUi(this);
    m_chartsModel.setCharts(charts);
    ui->charts->setModel(&m_chartsModel);
    ui->chartConfiguration->setChannels(channels);
    ui->chartConfiguration->setEnabled(false);

    connect(ui->chartConfiguration, &WidgetChartConfiguration::titleChanged, this, &WindowCharts::onTitleChartChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::xAxisChanged, this, &WindowCharts::onXAxisChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::yAxisChanged, this, &WindowCharts::onYAxisChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::yAxisAutorangeChanged, this, &WindowCharts::onYAxisAutorangeChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::yAxisMinChanged, this, &WindowCharts::onYAxisMinChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::yAxisMaxChanged, this, &WindowCharts::onYAxisMaxChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::samplesCountChanged, this, &WindowCharts::onSamplesCountChanged);
    connect(ui->chartConfiguration, &WidgetChartConfiguration::channelsChanged, this, &WindowCharts::onChannelsChanged);

}

WindowCharts::~WindowCharts()
{
    delete ui;
}

QList<Chart> WindowCharts::getCharts() const
{
    return m_chartsModel.charts();
}

void WindowCharts::onTitleChartChanged(const QString &title)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.setTitle(index, title);
}

void WindowCharts::onXAxisChanged(const QString &name)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setXAxis(name);
}

void WindowCharts::onYAxisChanged(const QString &name)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setYAxis(name);
}

void WindowCharts::onYAxisAutorangeChanged(bool enabled)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setYAxisAutorange(enabled);
}

void WindowCharts::onYAxisMinChanged(int value)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setYAxisMin(value);
}

void WindowCharts::onYAxisMaxChanged(int value)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setYAxisMax(value);
}

void WindowCharts::onSamplesCountChanged(int value)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setSamplesCount(value);
}

void WindowCharts::onChannelsChanged(const QList<int> &channels)
{
    auto index = ui->charts->selectionModel()->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    m_chartsModel.getChart(index).setChannels(channels);
}

void WindowCharts::on_ok_clicked()
{
    accept();
}

void WindowCharts::on_cancel_clicked()
{
    reject();
}

void WindowCharts::on_charts_clicked(const QModelIndex &index)
{
    ui->removeChart->setEnabled(true);
    auto& chart = m_chartsModel.getChart(index);
    ui->chartConfiguration->setChart(chart);
    ui->chartConfiguration->setEnabled(true);
}

void WindowCharts::on_addChart_clicked()
{
    m_chartsModel.addChart();
}

void WindowCharts::on_removeChart_clicked()
{
    auto index = ui->charts->selectionModel()->currentIndex();
    m_chartsModel.removeChart(index);
}
