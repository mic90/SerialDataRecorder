#include "ui/window_project.h"
#include "ui_window_project.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QsLog.h>
#include <external/qcustomplot/qcustomplot.h>

#include <ui/window_configuration.h>
#include <ui/window_variable.h>

WindowProject::WindowProject(QSharedPointer<Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowProject),
    m_project(project)
{
    ui->setupUi(this);
    m_varView.reset(new VariablesView(ui->variables, m_project));
    m_parsersManager.reset(new ParsersManager());
    m_parsersManager->loadPlugins();

    //TEST
    QCustomPlot *customPlot = new QCustomPlot();
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();
    customPlot->setMinimumHeight(300);
    ui->scrollAreaWidgetContents->layout()->addWidget(customPlot);
}

WindowProject::~WindowProject()
{
    delete ui;
}

QSharedPointer<Project> WindowProject::project() const
{
    return m_project;
}

void WindowProject::on_configuration_clicked()
{
    WindowConfiguration configWnd(m_project, m_parsersManager);
    if(configWnd.exec()) {
        m_project->setSerialConfig(configWnd.getSerialPortConfig());
        m_project->setDataParserName(configWnd.getDataParserName());
    }
}

void WindowProject::on_connect_clicked()
{
    if(m_project->getDataParserName().isEmpty()) {
        QMessageBox::critical(this, "Error", "No data parser was selected, please check project configuration");
        return;
    }

    m_parser = m_parsersManager->createParser(m_project->getDataParserName());
    QLOG_INFO() << "Data parser set to" << m_project->getDataParserName();
    m_serial.reset(new SerialPort(m_project->serialConfig(), m_parser));
    bool ret = m_serial->open();
    ui->connect->setDisabled(ret);
    ui->disconnect->setDisabled(!ret);
    if(!ret) {
        QMessageBox::critical(this, "Error", "Connection failed, can't open serial port");
        m_parser.reset();
        m_serial.reset();
        return;
    }
    connect(m_serial.data(), &SerialPort::dataReady, m_project.data(), &Project::setVariableValue);
}

void WindowProject::on_disconnect_clicked()
{
    bool ret = m_serial->close();
    ui->connect->setDisabled(!ret);
    ui->disconnect->setDisabled(ret);
    if(!ret) {
        QMessageBox::critical(this, "Error", "Can't close serial port");
        return;
    }
    m_parser.reset();
}

void WindowProject::on_addVar_clicked()
{
    WindowVariable varWnd;
    if(varWnd.exec()){
        m_project->addVariable(varWnd.getName(), varWnd.getId());
    }
}

void WindowProject::on_editVar_clicked()
{

}

void WindowProject::on_removeVar_clicked()
{

}

void WindowProject::on_moveVarDown_clicked()
{

}

void WindowProject::on_moveVarUp_clicked()
{

}

void WindowProject::on_addChart_clicked()
{

}

void WindowProject::on_editChart_clicked()
{

}

void WindowProject::on_removeChart_clicked()
{

}

void WindowProject::on_moveChartDown_clicked()
{

}

void WindowProject::on_moveChartUp_clicked()
{

}

void WindowProject::on_variables_itemSelectionChanged()
{
    if(!ui->variables->selectedItems().isEmpty()) {
        ui->editVar->setDisabled(false);
        ui->removeVar->setDisabled(false);
    } else {
        ui->editVar->setDisabled(true);
        ui->removeVar->setDisabled(true);
    }
}
