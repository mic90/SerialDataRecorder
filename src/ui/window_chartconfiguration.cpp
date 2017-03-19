#include "ui/window_chartconfiguration.h"
#include "ui_window_chartconfiguration.h"

WindowChartConfiguration::WindowChartConfiguration(Chart const& chart, const QList<Channel> &channels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowChartConfiguration),
    m_chart(chart)
{
    ui->setupUi(this);
    ui->chartConfiguration->setChannels(channels);
    ui->chartConfiguration->setChart(m_chart);
}

WindowChartConfiguration::~WindowChartConfiguration()
{
    delete ui;
}

void WindowChartConfiguration::on_ok_clicked()
{
    accept();
}

void WindowChartConfiguration::on_cancel_clicked()
{
    reject();
}
