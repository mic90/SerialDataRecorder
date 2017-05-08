#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QsLog.h>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_initialized(false)
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
    QMdiSubWindow *wnd = ui->mdiArea->currentSubWindow();
    if(wnd == nullptr) {
        return nullptr;
    }
    WindowProject *widget = qobject_cast<WindowProject*>(wnd->widget());
    if(widget == nullptr) {
        QLOG_ERROR() << "Unknown window widget";
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

bool MainWindow::openProject(const QString &path)
{
    QLOG_INFO() << "Loading project file" << path << "...";
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly))
    {
        QLOG_ERROR() << "Can't open file " << path << " for reading";
        return false;
    }
    auto json = f.readAll();
    auto project = QSharedPointer<Project>(new Project(path));
    if(!project->fromJson(json))
    {
        QLOG_ERROR() << "Can't parse project file";
        return false;
    }

    WindowProject *widget = new WindowProject(project);
    QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(widget);
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->setWindowIcon(QIcon());
    wnd->showMaximized();
    connect(widget, &WindowProject::savePossibleChanged, this, &MainWindow::onProjectSavePossible);

    return true;
}

void MainWindow::loadLastUsedProjects()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    auto paths = settings.value("openedProjects").toStringList();
    for(auto const& path : paths)
    {
        openProject(path);
    }
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
        QLOG_ERROR() << "Can't save project, no active window";
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

    if(!openProject(filePath))
    {
        QMessageBox::critical(this, "Error", "Can't open project file " + filePath);
    }
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
    QString path = QFileDialog::getSaveFileName(this, "Save chart images", QString(), "Images (*.png)");
    if(path.isEmpty())
    {
        return;
    }
    widget->exportImages(path);
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
    QStringList openedPaths;
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
        }
        openedPaths.append(widget->project()->path());
    }
    if(unsavedChanges)
    {
        auto ret = QMessageBox::question(this, "Unsaved changes", "Your changes in some files have not been saved.\nAre you sure you want to close the application ?");
        if(ret == QMessageBox::Yes)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    if(event->isAccepted())
    {
        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("openedProjects", openedPaths);
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    if(m_initialized)
    {
        return;
    }
    loadLastUsedProjects();
    m_initialized = true;
}

void MainWindow::on_actionExport_CSV_triggered()
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr)
    {
        return;
    }
    widget->setPause(true);
    QString path = QFileDialog::getSaveFileName(this, "Export to CSV", qApp->applicationDirPath(),  "CSV files (*.csv)");
    if(path.isEmpty())
    {
        widget->setPause(false);
        return;
    }
    widget->exportToFile(path);
    widget->setPause(false);
}

void MainWindow::on_actionImport_CSV_triggered()
{

}
