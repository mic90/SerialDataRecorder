#ifndef WINDOW_CONFIGURATION_H
#define WINDOW_CONFIGURATION_H

#include <QDialog>
#include <project/project.h>
#include <QSerialPort>

#include <parsers/parsersmanager.h>

namespace Ui {
class WindowConfiguration;
}

class WindowConfiguration : public QDialog
{
    Q_OBJECT

public:
    WindowConfiguration(QSharedPointer<Project> project, QSharedPointer<ParsersManager> parsersManager,
                                 QWidget *parent = 0);
    ~WindowConfiguration();

    SerialPortConfig getSerialPortConfig();
    QString getDataParserName();

private:
    void setPortName(const QString& port);
    QString getPortName() const;
    QSerialPort::BaudRate getBaudRate() const;
    void setBaudRate(QSerialPort::BaudRate);
    QStringList getSerialPorts();

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();
    void on_parsers_currentTextChanged(const QString &arg1);

private:
    Ui::WindowConfiguration *ui;
    QSharedPointer<Project> m_project;
    SerialPortConfig m_config;
    QSharedPointer<ParsersManager> m_parsersManager;
};

#endif // WINDOW_CONFIGURATION_H
