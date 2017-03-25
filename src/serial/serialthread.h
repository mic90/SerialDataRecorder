#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>
#include <serial/serialport.h>

class SerialThread : public QObject
{
    Q_OBJECT
public:
    SerialThread(const SerialPortConfig& config, std::unique_ptr<DataParserBase> parser, QObject *parent = 0);
    ~SerialThread();

    bool open();
    void close();

public slots:
    void process();

signals:
    void dataReady(QList<QJsonArray>);
    void startSerialProcessing();

private:
    QScopedPointer<SerialPort> m_port;
    QThread m_thread;
};

#endif // SERIALTHREAD_H
