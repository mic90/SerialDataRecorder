#ifndef WINDOW_CONFIGURATION_H
#define WINDOW_CONFIGURATION_H

#include <QDialog>
#include <project/project.h>
#include <QSerialPort>

#include <parsers/dataparserfactory.h>

namespace Ui {
class WindowConfiguration;
}

class WindowConfiguration : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowConfiguration)
public:
    WindowConfiguration(QSharedPointer<Project> project, QStringList const& parserNames, QWidget *parent = 0);
    ~WindowConfiguration();

    SerialPortConfig getSerialPortConfig();
    QString getDataParser();

private:
    void setPortName(const QString& port);
    QString getPortName() const;
    QSerialPort::BaudRate getBaudRate() const;
    void setBaudRate(QSerialPort::BaudRate);
    QStringList getSerialPorts();

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();

private:
    Ui::WindowConfiguration *ui;
    QSharedPointer<Project> m_project;
    SerialPortConfig m_config;
};

#endif // WINDOW_CONFIGURATION_H
