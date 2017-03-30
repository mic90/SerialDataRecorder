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

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private:
    WindowProject *getActiveWidget();
    bool saveProject(QSharedPointer<Project> const& project);
    bool openProject(QString const& path);
    void loadLastUsedProjects();

private slots:
    void onProjectSavePossible(bool);
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionExport_Image_triggered();
    void on_actionNight_view_toggled(bool arg1);
    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

private:
    Ui::MainWindow *ui;
    bool m_initialized;
};

#endif // MAINWINDOW_H
