#include "serialthread.h"

SerialThread::SerialThread(const SerialPortConfig &config, std::unique_ptr<DataParserBase> parser, QObject *parent):
    QObject(parent),
    m_port(new SerialPort(config, std::move(parser)))
{
    m_thread.setObjectName("SerialThread");
    m_port->moveToThread(&m_thread);
    m_thread.start();

    connect(m_port.data(), &SerialPort::dataReady, this, &SerialThread::dataReady);
    connect(this, &SerialThread::startSerialProcessing, m_port.data(), &SerialPort::process);
}

SerialThread::~SerialThread()
{
    m_port->close();
    m_thread.quit();
    m_thread.wait();
}

bool SerialThread::open()
{
    return m_port->open();
}

void SerialThread::close()
{
    m_port->close();
}

void SerialThread::process()
{
    emit startSerialProcessing();
}
