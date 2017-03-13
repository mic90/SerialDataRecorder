#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QsLog.h>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_projectMgr()
{
    ui->setupUi(this);
    m_view.reset(new MultiWindowView(ui->mdiArea));
    connect(&m_projectMgr, &ProjectManager::projectCreated, m_view.data(), &MultiWindowView::onProjectCreated);
    connect(&m_projectMgr, &ProjectManager::projectOpened, m_view.data(), &MultiWindowView::onProjectCreated);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    m_projectMgr.createProject();
}

void MainWindow::on_actionSave_triggered()
{
    m_view->save();
}

void MainWindow::on_actionSave_as_triggered()
{
    m_view->saveAs();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open project", qApp->applicationDirPath(), "SDR Files (*.sdr)");
    if(filePath.isEmpty()) {
        return;
    }
    bool ret = m_projectMgr.openProject(filePath);
    if(!ret) {
        QMessageBox::critical(this, "Error", "Can't open project file: " + filePath);
    }
}

void MainWindow::on_actionSettings_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{

}
