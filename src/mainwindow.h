#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui/window_project.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    WindowProject *getActiveWidget();

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();

    void on_actionExport_Image_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
