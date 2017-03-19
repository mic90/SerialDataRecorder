#include "ui/window_configuration.h"
#include "ui_window_configuration.h"

#include <QSerialPortInfo>

#include <serial/serialportconfig.h>

WindowConfiguration::WindowConfiguration(QSharedPointer<Project> project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowConfiguration),
    m_project(project)
{
    ui->setupUi(this);
    ui->name->addItems(getSerialPorts());

    m_config = project->serialConfig();
    setBaudRate((QSerialPort::BaudRate)m_config.baudRate());
    setPortName(m_config.name());
    ui->dataBits->setCurrentText(QString::number(m_config.dataBits()));
}

WindowConfiguration::~WindowConfiguration()
{
    delete ui;
}

SerialPortConfig WindowConfiguration::getSerialPortConfig()
{
    m_config.setName(getPortName());
    m_config.setBaudRate(getBaudRate());
    m_config.setDataBits(ui->dataBits->currentText().toInt());
    return m_config;
}

QString WindowConfiguration::getDataFormat()
{
    return ui->dataFormat->currentText();
}

void WindowConfiguration::setPortName(const QString &port)
{
    ui->name->setCurrentText(port);
}

QString WindowConfiguration::getPortName() const
{
    return ui->name->currentText();
}

QSerialPort::BaudRate WindowConfiguration::getBaudRate() const
{
    int index = ui->baud->currentIndex();
    if(index == 0) {
        return QSerialPort::Baud1200;
    } else if(index == 1) {
        return QSerialPort::Baud2400;
    } else if(index == 2) {
        return QSerialPort::Baud4800;
    } else if(index == 3) {
        return QSerialPort::Baud9600;
    } else if(index == 4) {
        return QSerialPort::Baud19200;
    } else if(index == 5) {
        return QSerialPort::Baud38400;
    } else if(index == 6) {
        return QSerialPort::Baud57600;
    } else if(index == 7) {
        return QSerialPort::Baud115200;
    } else {
        return QSerialPort::Baud9600;
    }
}

void WindowConfiguration::setBaudRate(QSerialPort::BaudRate index)
{
    if(index == QSerialPort::Baud1200) {
        return ui->baud->setCurrentIndex(0);
    } else if(index == QSerialPort::Baud2400) {
        return  ui->baud->setCurrentIndex(1);
    } else if(index == QSerialPort::Baud4800) {
        return  ui->baud->setCurrentIndex(2);
    } else if(index == QSerialPort::Baud9600) {
        return  ui->baud->setCurrentIndex(3);
    } else if(index == QSerialPort::Baud19200) {
        return  ui->baud->setCurrentIndex(4);
    } else if(index == QSerialPort::Baud38400) {
        return  ui->baud->setCurrentIndex(5);
    } else if(index == QSerialPort::Baud57600) {
        return ui->baud->setCurrentIndex(6);
    } else if(index == QSerialPort::Baud115200) {
        return ui->baud->setCurrentIndex(7);
    } else {
        return ui->baud->setCurrentIndex(3);
    }
}

QStringList WindowConfiguration::getSerialPorts()
{
    QStringList portNames;
    for(auto portInfo : QSerialPortInfo::availablePorts())
    {
        portNames.append(portInfo.portName());
    }
    return portNames;
}

void WindowConfiguration::on_ok_clicked()
{
    accept();
}

void WindowConfiguration::on_cancel_clicked()
{
    reject();
}
