#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <project/projectmanager.h>
#include <views/multiwindowview.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionOpen_triggered();
    void on_actionSettings_triggered();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    ProjectManager m_projectMgr;
    QScopedPointer<MultiWindowView> m_view;
};

#endif // MAINWINDOW_H
