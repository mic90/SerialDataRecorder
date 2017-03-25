#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QsLog.h>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QThread::currentThread()->setObjectName("MainThread");

    qRegisterMetaType<QList<QJsonArray>>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

WindowProject *MainWindow::getActiveWidget()
{
    QMdiSubWindow *wnd = ui->mdiArea->activeSubWindow();
    if(wnd == nullptr) {
        return nullptr;
    }
    WindowProject *widget = qobject_cast<WindowProject*>(wnd->widget());
    if(widget == nullptr) {
        return nullptr;
    }
    return widget;
}

void MainWindow::on_actionNew_triggered()
{
    QLOG_DEBUG() << "Creating new project";
    auto newProject = QSharedPointer<Project>(new Project());
    WindowProject *widget = new WindowProject(newProject);
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(widget);
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->setWindowIcon(QIcon());
    wnd->show();
}

void MainWindow::on_actionSave_triggered()
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr)
    {
        return;
    }
    if(widget->project()->path().isEmpty())
    {
        QString path = QFileDialog::getSaveFileName(ui->mdiArea->parentWidget(), "Save project",
                                                    qApp->applicationDirPath(), "SDR Files (*.sdr)");
        if(path.isEmpty())
        {
            return;
        }
        widget->project()->save(path);
        widget->setWindowTitle(widget->project()->name());
        return;
    }
    widget->project()->save();
}

void MainWindow::on_actionSave_as_triggered()
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr)
    {
        return;
    }
    QString path = QFileDialog::getSaveFileName(ui->mdiArea->parentWidget(), "Save project as...",
                                                qApp->applicationDirPath(), "SDR Files (*.sdr)");
    if(path.isEmpty())
    {
        return;
    }
    widget->project()->save(path);
    widget->setWindowTitle(widget->project()->name());
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open project", qApp->applicationDirPath(), "SDR Files (*.sdr)");
    if(filePath.isEmpty())
    {
        return;
    }
    for(auto subWindow : ui->mdiArea->subWindowList())
    {
        WindowProject *widget = qobject_cast<WindowProject*>(subWindow->widget());
        if(widget == nullptr)
        {
            continue;
        }
        if(widget->project()->path() == filePath)
        {
            ui->mdiArea->setActiveSubWindow(subWindow);
            return;
        }
    }

    auto project = QSharedPointer<Project>(new Project(filePath));
    if(!project->load())
    {
        QMessageBox::critical(this, "Error", "Can't open project file: " + filePath);
        return;
    }

    QLOG_DEBUG() << "Opening project " << project->name();
    WindowProject *widget = new WindowProject(project);
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(widget);
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->setWindowIcon(QIcon());
    wnd->show();
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionExport_Image_triggered()
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr)
    {
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "Save chart images", QString(), "Images (*.png)");
    widget->exportImages(filePath);
}

void MainWindow::on_actionNight_view_toggled(bool enabled)
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr)
    {
        return;
    }
    widget->setNightView(enabled);
}
