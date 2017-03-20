#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <serial/serialportconfig.h>
#include <parsers/dataparser.h>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort(const SerialPortConfig& config, std::unique_ptr<DataParserBase> parser, QObject *parent = 0);

    bool open();
    bool close();

public slots:
    void process();

signals:
    void dataReady(QJsonArray);

private:
    SerialPortConfig m_config;
    QSerialPort m_serial;
    std::unique_ptr<DataParserBase> m_parser;
    int m_lastTimestamp;
};

#endif // SERIALPORT_H
