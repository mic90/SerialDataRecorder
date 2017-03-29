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

bool MainWindow::saveProject(const QSharedPointer<Project> &project)
{
    QLOG_INFO() << "Saving project to" << project->path() << "...";
    QFile f(project->path());
    if(!f.open(QIODevice::WriteOnly))
    {
        QLOG_ERROR() << "Can't open file" << project->path() << "for saving";
        return false;
    }
    f.write(project->toJson());
    return true;
}

void MainWindow::onProjectSavePossible(bool savePossible)
{
    ui->actionSave->setEnabled(savePossible);
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
    connect(widget, &WindowProject::savePossibleChanged, this, &MainWindow::onProjectSavePossible);
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
        widget->project()->setPath(path);
        if(!saveProject(widget->project()))
        {
            QMessageBox::critical(this, "Error", "Couldn't save project to " + path);
            return;
        }
        widget->setWindowTitle(widget->project()->name());
        return;
    }
    saveProject(widget->project());
    widget->setHasUnsavedChanges(false);
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
    widget->project()->setPath(path);
    if(!saveProject(widget->project()))
    {
        QMessageBox::critical(this, "Error", "Can't save project to " + path);
        return;
    }
    widget->setHasUnsavedChanges(false);
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
    QLOG_INFO() << "Loading project file" << filePath << "...";
    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly))
    {
        QString message{"Can't open file " + filePath + " for reading"};
        QLOG_ERROR() << message;
        QMessageBox::critical(this, "Error", message);
        return;
    }
    auto json = f.readAll();
    auto project = QSharedPointer<Project>(new Project(filePath));
    if(!project->fromJson(json))
    {
        QMessageBox::critical(this, "Error", "Can't parse project file");
        return;
    }

    WindowProject *widget = new WindowProject(project);
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(widget);
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->setWindowIcon(QIcon());
    wnd->show();

    connect(widget, &WindowProject::savePossibleChanged, this, &MainWindow::onProjectSavePossible);
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

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *subWindow)
{
    if(subWindow == nullptr)
    {
        ui->actionExport_CSV->setDisabled(true);
        ui->actionExport_Image->setDisabled(true);
        ui->actionSave->setDisabled(true);
        return;
    }
    ui->actionExport_CSV->setDisabled(false);
    ui->actionExport_Image->setDisabled(false);
    auto widget = qobject_cast<WindowProject*>(subWindow->widget());
    if(widget == nullptr)
    {
        return;
    }
    ui->actionSave->setDisabled(!widget->hasUnsavedChanges());
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    bool unsavedChanges = false;
    for(auto subWindow : ui->mdiArea->subWindowList())
    {
        auto widget = qobject_cast<WindowProject*>(subWindow->widget());
        if(widget == nullptr)
        {
            continue;
        }
        if(widget->hasUnsavedChanges())
        {
            unsavedChanges = true;
            break;
        }
    }
    if(unsavedChanges)
    {
        auto ret = QMessageBox::question(this, "Unsaved changes", "Your changes in some files have not been saved.\nAre you sure you want to close the application ?");
        if(ret == QMessageBox::Yes)
        {
            event->accept();
            return;
        }
        event->ignore();
    }
}
