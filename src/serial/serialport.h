#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <serial/serialportconfig.h>
#include <plugins/idataparser.h>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort(const SerialPortConfig& config, QSharedPointer<DataParserBase> parser, QObject *parent = 0);

    bool open();
    bool close();

private slots:
    void onDataReady();
    void onDataWritten(qint64);

signals:
    void dataReady(QJsonObject);

private:
    SerialPortConfig m_config;
    QSerialPort m_serial;
    QSharedPointer<DataParserBase> m_parser;
};

#endif // SERIALPORT_H
