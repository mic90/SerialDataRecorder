#include "serialport.h"
#include <QsLog.h>
#include <QThread>

SerialPort::SerialPort(const SerialPortConfig &config, std::unique_ptr<DataParserBase> parser, QObject *parent) : QObject(parent),
    m_config(config),
    m_parser(parser.release()),
    m_buffer(""),
    m_pause(false)
{
    qRegisterMetaType<QList<QJsonArray>>("QList<QJsonArray>");
}

SerialPort::~SerialPort()
{
    close();
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
    connect(&m_serial, &QSerialPort::errorOccurred, this, &SerialPort::onSerialError);
    return true;
}

void SerialPort::close()
{
    if(!m_serial.isOpen())
    {
        return;
    }

    m_serial.clear();
    m_serial.close();
    QLOG_INFO() << "Serial port" << m_config.name() << "closed";
}

void SerialPort::setPause(bool pause)
{
    m_pause = pause;
}

bool SerialPort::isOpen() const
{
    return m_serial.isOpen();
}

void SerialPort::process()
{
    while(m_serial.isOpen())
    {
        if(m_serial.bytesAvailable() <= 0)
        {
            QThread::usleep(100);
            continue;
        }

        QString data = QString::fromUtf8(m_serial.readLine());
        if(m_pause)
        {
            continue;
        }
        auto parsed = m_parser->parse(data);
        if(!parsed.isEmpty()) {
            emit dataReady(parsed);
        }
        QThread::usleep(100);
    }
    QLOG_DEBUG() << "Serial port processing finished";
}

void SerialPort::onSerialError(QSerialPort::SerialPortError e)
{
    QLOG_ERROR() << "Serial port error:" << e;
}
