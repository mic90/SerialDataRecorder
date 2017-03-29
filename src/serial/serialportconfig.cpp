#include "serialportconfig.h"
#include <QSerialPort>

SerialPortConfig::SerialPortConfig() :
    m_name(""),
    m_baudRate(QSerialPort::Baud9600),
    m_dataBits(QSerialPort::Data8),
    m_parity(QSerialPort::NoParity),
    m_stopBits(QSerialPort::OneStop),
    m_flowControl(QSerialPort::NoFlowControl)
{

}

QJsonObject SerialPortConfig::toJson()
{
    QJsonObject obj;
    obj.insert("name", m_name);
    obj.insert("baudRate", m_baudRate);
    obj.insert("dataBits", m_dataBits);
    obj.insert("parity", m_parity);
    obj.insert("stopBits", m_stopBits);
    obj.insert("flowControl", m_flowControl);
    return obj;
}

bool SerialPortConfig::fromJson(const QJsonObject &json)
{
    if(!json.contains("name")
            || !json.contains("baudRate")
            || !json.contains("dataBits")
            || !json.contains("parity")
            || !json.contains("stopBits")
            || !json.contains("flowControl")) {
        return false;
    }
    m_name = json.value("name").toString();
    m_baudRate = json.value("baudRate").toInt();
    m_dataBits = json.value("dataBits").toInt();
    m_parity = json.value("parity").toInt();
    m_stopBits = json.value("stopBits").toInt();
    m_flowControl = json.value("flowControl").toInt();
    return true;
}

bool SerialPortConfig::operator ==(const SerialPortConfig &other)
{
    return m_name == other.m_name &&
            m_baudRate == other.m_baudRate &&
            m_dataBits == other.m_dataBits &&
            m_parity == other.m_parity &&
            m_stopBits == other.m_stopBits &&
            m_flowControl == other.m_flowControl;
}

QString SerialPortConfig::name() const
{
    return m_name;
}

void SerialPortConfig::setName(const QString &name)
{
    m_name = name;
}

int SerialPortConfig::baudRate() const
{
    return m_baudRate;
}

void SerialPortConfig::setBaudRate(int baudRate)
{
    m_baudRate = baudRate;
}

int SerialPortConfig::dataBits() const
{
    return m_dataBits;
}

void SerialPortConfig::setDataBits(int dataBits)
{
    m_dataBits = dataBits;
}

int SerialPortConfig::parity() const
{
    return m_parity;
}

void SerialPortConfig::setParity(int parity)
{
    m_parity = parity;
}

int SerialPortConfig::stopBits() const
{
    return m_stopBits;
}

void SerialPortConfig::setStopBits(int stopBits)
{
    m_stopBits = stopBits;
}

int SerialPortConfig::flowControl() const
{
    return m_flowControl;
}

void SerialPortConfig::setFlowControl(int flowControl)
{
    m_flowControl = flowControl;
}
