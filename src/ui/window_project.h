#ifndef WINDOW_PROJECT_H
#define WINDOW_PROJECT_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <project/project.h>
#include <serial/serialport.h>
#include <views/variablesview.h>
#include <parsers/parsersmanager.h>

namespace Ui {
class WindowProject;
}

class WindowProject : public QMainWindow
{
    Q_OBJECT

public:
    WindowProject(QSharedPointer<Project> project, QWidget *parent = 0);
    ~WindowProject();

    QSharedPointer<Project> project() const;

private slots:
    void on_configuration_clicked();
    void on_connect_clicked();
    void on_disconnect_clicked();

    void on_addVar_clicked();
    void on_editVar_clicked();
    void on_removeVar_clicked();
    void on_moveVarDown_clicked();
    void on_moveVarUp_clicked();

    void on_addChart_clicked();
    void on_editChart_clicked();
    void on_removeChart_clicked();
    void on_moveChartDown_clicked();
    void on_moveChartUp_clicked();

    void on_variables_itemSelectionChanged();

private:
    Ui::WindowProject *ui;
    QSharedPointer<Project> m_project;
    QSharedPointer<SerialPort> m_serial;
    QSharedPointer<VariablesView> m_varView;
    QSharedPointer<ParsersManager> m_parsersManager;
    QSharedPointer<DataParserBase> m_parser;
};

#endif // WINDOW_PROJECT_H
