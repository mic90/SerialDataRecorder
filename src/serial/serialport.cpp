#include "serialport.h"
#include <QsLog.h>

SerialPort::SerialPort(const SerialPortConfig &config, std::unique_ptr<DataParserBase> parser, QObject *parent) : QObject(parent),
    m_config(config),
    m_parser(parser.release()),
    m_lastTimestamp(0)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialPort::onDataReady);
    connect(&m_serial, &QSerialPort::bytesWritten, this, &SerialPort::onDataWritten);
}

bool SerialPort::open()
{
    if(m_config.name().isEmpty())
    {
        QLOG_ERROR() << "Serial port is not set";
        return false;
    }
    m_serial.setPortName(m_config.name());
    if(!m_serial.open(QIODevice::ReadWrite))
    {
        QLOG_ERROR() << "Can't open serial port" << m_config.name();
        return false;
    }
    m_serial.setBaudRate(m_config.baudRate());
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    m_serial.setParity(QSerialPort::NoParity);
    QLOG_INFO() << "Serial port" << m_config.name() << "opened!";
    return true;
}

bool SerialPort::close()
{
    m_serial.close();
    QLOG_INFO() << "Serial port" << m_config.name() << "closed";
    return true;
}

void SerialPort::onDataReady()
{
    auto timestamp = QTime::currentTime().msecsSinceStartOfDay();
    QLOG_DEBUG() << timestamp - m_lastTimestamp;
    m_lastTimestamp = timestamp;

    QString data = m_serial.readAll();
    QJsonArray ret = m_parser->parse(data);
    if(!ret.isEmpty())
    {
        emit dataReady(ret);
    }
}

void SerialPort::onDataWritten(qint64)
{

}
