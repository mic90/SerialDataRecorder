#include "serialport.h"
#include <QsLog.h>
#include <QThread>

SerialPort::SerialPort(const SerialPortConfig &config, std::unique_ptr<DataParserBase> parser, QObject *parent) : QObject(parent),
    m_config(config),
    m_parser(parser.release()),
    m_lastTimestamp(0)
{
    connect(m_parser.get(), &DataParserBase::dataReady, this, &SerialPort::dataReady);
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

void SerialPort::process()
{
    while(m_serial.isOpen())
    {
        QString data = m_serial.readAll();
        if(data.isEmpty())
        {
            continue;
        }
        m_parser->parse(data);
    }
    QLOG_DEBUG() << "Serial port processing finished";
}
