#include "ui/window_project.h"
#include "ui_window_project.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QsLog.h>
#include <external/qcustomplot/qcustomplot.h>

#include <ui/window_configuration.h>
#include <ui/window_channels.h>
#include <ui/window_charts.h>
#include <ui/window_chartconfiguration.h>
#include <parsers/dataparserfactory.h>

WindowProject::WindowProject(QSharedPointer<Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowProject),
    m_project(project)
{
    ui->setupUi(this);

    ui->toolBar->addAction(ui->actionConnect);
    ui->toolBar->addAction(ui->actionPause);
    ui->toolBar->addAction(ui->actionDisconnect);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSerial_Configuration);
    ui->toolBar->addAction(ui->actionChannels);
    ui->toolBar->addAction(ui->actionCharts);

    createCharts(m_project->getCharts(), m_project->getChannels());
}

WindowProject::~WindowProject()
{
    delete ui;
}

QSharedPointer<Project> WindowProject::project() const
{
    return m_project;
}

void WindowProject::exportImages(const QString &fileName)
{
    for(auto chartWidget : m_charts)
    {
        chartWidget->exportImage(fileName);
    }
}

void WindowProject::createChart(const Chart &chart, const QList<Channel> &channels)
{
    ChartWidget *widget = new ChartWidget(chart, channels, this);
    ui->scrollAreaLayout->addWidget(widget);
    m_charts.append(widget);
    connect(widget, &ChartWidget::minimumHeightChanged, this, &WindowProject::onChartMinimumHeightChanged);
    connect(widget, &ChartWidget::editRequested, this, &WindowProject::onChartEditRequested);
}

void WindowProject::createCharts(const QList<Chart> &charts, QList<Channel> const& channels)
{
    while(m_charts.size() > 0)
    {
        delete m_charts.takeFirst();
    }
    for(auto const& chart: charts)
    {
        createChart(chart, channels);
    }
}

void WindowProject::clearCharts()
{
    for(auto chartWidget : m_charts)
    {
        chartWidget->clearData();
    }
}

void WindowProject::onChartMinimumHeightChanged(int height)
{
    ChartWidget *widget = qobject_cast<ChartWidget*>(sender());
    if(widget == nullptr)
    {
        return;
    }
    auto index = m_charts.indexOf(widget);
    auto charts = m_project->getCharts();
    charts[index].setMinimumHeight(height);
    m_project->setCharts(charts);
}

void WindowProject::onChartEditRequested()
{
    ChartWidget *widget = qobject_cast<ChartWidget*>(sender());
    if(widget == nullptr)
    {
        return;
    }
    auto index = m_charts.indexOf(widget);
    auto chart = m_project->getCharts().at(index);
    WindowChartConfiguration wndChartConfig(chart, m_project->getChannels());
    if(wndChartConfig.exec())
    {

    }
}

void WindowProject::on_actionConnect_triggered()
{
    clearCharts();

    if(m_project->getDataParserName().isEmpty())
    {
        QLOG_ERROR() << "No data parser was selected, please check project configuration";
        QMessageBox::critical(this, "Error", "No data parser was selected, please check project configuration");
        return;
    }

    auto parser = m_parserFactory.createParser("ASCII");
    if(parser == nullptr)
    {
        QLOG_ERROR() << "Couldn't create given data parser";
        QMessageBox::critical(this, "Error", "Couldn't create given data parser");
        return;
    }
    QLOG_INFO() << "Data parser set to" << m_project->getDataParserName();
    m_serial.reset(new SerialPort(m_project->serialConfig(), std::move(parser)));
    bool ret = m_serial->open();
    if(!ret)
    {
        QMessageBox::critical(this, "Error", "Connection failed, can't open serial port");
        m_serial.reset();
        return;
    }
    ui->actionConnect->setDisabled(ret);
    ui->actionPause->setDisabled(!ret);
    ui->actionDisconnect->setDisabled(!ret);
    for(auto chartWidget : m_charts)
    {
        connect(m_serial.data(), &SerialPort::dataReady, chartWidget, &ChartWidget::setData);
    }
    setWindowTitle(QString("%1 [%2]").arg(m_project->name(), m_project->serialConfig().name()));
}

void WindowProject::on_actionPause_triggered()
{
    for(auto chartWidget : m_charts)
    {
        chartWidget->setPause(ui->actionPause->isChecked());
    }
}

void WindowProject::on_actionDisconnect_triggered()
{
    bool ret = m_serial->close();
    if(!ret)
    {
        QMessageBox::critical(this, "Error", "Can't close serial port");
        return;
    }

    ui->actionConnect->setDisabled(!ret);
    ui->actionPause->setDisabled(!ret);
    ui->actionDisconnect->setDisabled(ret);
    setWindowTitle(m_project->name());
}

void WindowProject::on_actionSerial_Configuration_triggered()
{
    WindowConfiguration configWnd(m_project);
    if(configWnd.exec())
    {
        m_project->setSerialConfig(configWnd.getSerialPortConfig());
        m_project->setDataParserName(configWnd.getDataFormat());
    }
}

void WindowProject::on_actionChannels_triggered()
{
    WindowChannels channelsWnd(m_project->getChannels());
    if(channelsWnd.exec())
    {
        auto const channels = channelsWnd.getChannels();
        for(auto *chart : m_charts)
        {
            chart->setChannels(channels);
        }
        m_project->setChannels(channels);
    }
}

void WindowProject::on_actionCharts_triggered()
{
    WindowCharts chartsWnd(m_project->getCharts(), m_project->getChannels());
    if(chartsWnd.exec())
    {
        auto charts = chartsWnd.getCharts();
        while(m_charts.size() != charts.size())
        {
            if(m_charts.size() > charts.size())
            {
                delete m_charts.takeLast();
            }
            else
            {
                auto chart = charts.at(m_charts.size());
                createChart(chart, m_project->getChannels());
            }
        }
        for(int i=0; i<charts.size(); i++)
        {
            m_charts.at(i)->setChart(charts.at(i));
        }
        m_project->setCharts(charts);
    }
}

