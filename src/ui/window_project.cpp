#include "ui/window_project.h"
#include "ui_window_project.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QsLog.h>
#include <QFileInfo>
#include <external/qcustomplot/qcustomplot.h>

#include <ui/window_configuration.h>
#include <ui/window_channels.h>
#include <ui/window_charts.h>
#include <ui/window_chartconfiguration.h>
#include <parsers/dataparserfactory.h>

WindowProject::WindowProject(QSharedPointer<Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowProject),
    m_project(project),
    m_hasUnsavedChanges(false)
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
    updateWindowTitle();

    connect(project.data(), &Project::projectChanged, this, &WindowProject::onProjectChanged);
}

WindowProject::~WindowProject()
{
    delete ui;
}

QSharedPointer<Project> WindowProject::project() const
{
    return m_project;
}

void WindowProject::exportImages(const QString &filePath)
{
    QFileInfo fInfo(filePath);
    QString fileDir = fInfo.absolutePath();
    QString fileBase = fInfo.baseName();

    int index = 0;
    for(auto chartWidget : m_charts)
    {
        QString path = QString("%1/%2_%3.png").arg(fileDir).arg(fileBase).arg(index++);
        chartWidget->exportImage(path);
    }
}

void WindowProject::setNightView(bool enabled)
{
    for(auto chartWidget : m_charts)
    {
        if(enabled)
        {
            chartWidget->setDarkTheme();
        }
        else
        {
            chartWidget->setLightTheme();
        }
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

void WindowProject::updateWindowTitle()
{
    QString asterisk = m_hasUnsavedChanges ? "*" : "";
    if(m_serial != nullptr && m_serial->isOpen())
    {
        setWindowTitle(QString("%1 [%2]%3").arg(m_project->name(), m_project->serialConfig().name(), asterisk));
    }
    else
    {
        setWindowTitle(QString("%1%2").arg(m_project->name(), asterisk));
    }
}

void WindowProject::onProjectChanged()
{
    setHasUnsavedChanges(true);
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
    m_serial.reset(new SerialThread(m_project->serialConfig(), std::move(parser)));
    connect(this, &WindowProject::startProcessing, m_serial.data(), &SerialThread::process);
    bool ret = m_serial->open();
    if(!ret)
    {
        QMessageBox::critical(this, "Error", "Connection failed, can't open serial port");
        return;
    }
    emit startProcessing();

    ui->actionConnect->setDisabled(true);
    ui->actionPause->setDisabled(false);
    ui->actionDisconnect->setDisabled(false);
    ui->actionSerial_Configuration->setDisabled(true);
    for(auto chartWidget : m_charts)
    {
        connect(m_serial.data(), &SerialThread::dataReady, chartWidget, &ChartWidget::setData, Qt::QueuedConnection);
    }
    updateWindowTitle();
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
    m_serial->close();
    ui->actionConnect->setDisabled(false);
    ui->actionPause->setDisabled(true);
    ui->actionDisconnect->setDisabled(true);
    ui->actionSerial_Configuration->setDisabled(false);
    updateWindowTitle();
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

bool WindowProject::hasUnsavedChanges() const
{
    return m_hasUnsavedChanges;
}

void WindowProject::setHasUnsavedChanges(bool hasUnsavedChanges)
{
    m_hasUnsavedChanges = hasUnsavedChanges;
    updateWindowTitle();
    emit savePossibleChanged(m_hasUnsavedChanges);
}

void WindowProject::closeEvent(QCloseEvent *event)
{
    if(!m_hasUnsavedChanges)
    {
        event->accept();
        return;
    }
    auto ret = QMessageBox::question(this, "Unsaved changes", "Your changes have not been saved.\nAre you sure you want to close the project window ?");
    if(ret == QMessageBox::Yes)
    {
        event->accept();
        return;
    }
    event->ignore();
}
