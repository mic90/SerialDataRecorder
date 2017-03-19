#include "ui/widget_chartconfiguration.h"
#include "ui_widget_chartconfiguration.h"

#include <QListWidgetItem>
#include <QDebug>

WidgetChartConfiguration::WidgetChartConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChartConfiguration)
{
    ui->setupUi(this);
}

WidgetChartConfiguration::~WidgetChartConfiguration()
{
    delete ui;
}

void WidgetChartConfiguration::setChart(const Chart &chart)
{
    ui->title->setText(chart.name());
    ui->xAxis->setText(chart.xAxis());
    ui->yAxis->setText(chart.yAxis());
    ui->autorange->setChecked(chart.yAxisAutorange());
    ui->samplesCount->setValue(chart.samplesCount());
    ui->yAxisMin->setValue(chart.yAxisMin());
    ui->yAxisMax->setValue(chart.yAxisMax());
    for(int i=0; i<m_channelItems.size(); i++)
    {
        m_channelItems.at(i)->setCheckState(chart.channels().contains(i) ? Qt::Checked : Qt::Unchecked);
    }
}

void WidgetChartConfiguration::setChannels(const QList<Channel> &channels)
{
    for(auto const& channel : channels)
    {
        QListWidgetItem *newItem = new QListWidgetItem(channel.name(), ui->channels);
        newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
        newItem->setIcon(QIcon(":/icons/variables/tag_blue.png"));
        newItem->setCheckState(Qt::Unchecked);
        m_channelItems.append(newItem);
    }
}

void WidgetChartConfiguration::on_autorange_toggled(bool checked)
{
    ui->yAxisMin->setEnabled(!checked);
    ui->yAxisMax->setEnabled(!checked);

    emit yAxisAutorangeChanged(checked);
}

void WidgetChartConfiguration::on_title_editingFinished()
{
    emit titleChanged(ui->title->text());
}

void WidgetChartConfiguration::on_xAxis_editingFinished()
{
    emit xAxisChanged(ui->xAxis->text());
}

void WidgetChartConfiguration::on_yAxis_editingFinished()
{
    emit yAxisChanged(ui->yAxis->text());
}

void WidgetChartConfiguration::on_samplesCount_valueChanged(int value)
{
    emit samplesCountChanged(value);
}

void WidgetChartConfiguration::on_yAxisMin_valueChanged(int value)
{
    emit yAxisMinChanged(value);
}

void WidgetChartConfiguration::on_yAxisMax_valueChanged(int value)
{
    emit yAxisMaxChanged(value);
}

void WidgetChartConfiguration::on_channels_clicked(const QModelIndex &)
{
    auto rows = ui->channels->model()->rowCount();
    QList<int> selectedChannels;
    for(int i=0; i<rows; i++)
    {
        bool checked = ui->channels->model()->data(ui->channels->model()->index(i, 0), Qt::CheckStateRole) == Qt::Checked;
        if(checked)
        {
            selectedChannels.append(i);
        }
    }
    emit channelsChanged(selectedChannels);
}
