#ifndef SERIALPORTCONFIG_H
#define SERIALPORTCONFIG_H

#include <QString>
#include <QJsonObject>

class SerialPortConfig
{
public:
    SerialPortConfig();

    QJsonObject serialize();
    bool deserialize(const QJsonObject& json);

    QString name() const;
    void setName(const QString &name);
    int baudRate() const;
    void setBaudRate(int baudRate);
    int dataBits() const;
    void setDataBits(int dataBits);
    int parity() const;
    void setParity(int parity);
    int stopBits() const;
    void setStopBits(int stopBits);
    int flowControl() const;
    void setFlowControl(int flowControl);

private:
    QString m_name;
    int m_baudRate;
    int m_dataBits;
    int m_parity;
    int m_stopBits;
    int m_flowControl;
};

#endif // SERIALPORTCONFIG_H
