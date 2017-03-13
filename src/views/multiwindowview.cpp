#include "multiwindowview.h"
#include <QsLog.h>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QApplication>


MultiWindowView::MultiWindowView(QMdiArea *mdi, QObject *parent) : QObject(parent),
    m_mdi(mdi)
{
    connect(m_mdi, &QMdiArea::subWindowActivated, this, &MultiWindowView::onActiveWindowChanged);
}

bool MultiWindowView::save()
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr) {
        return false;
    }
    if(widget->project()->path().isEmpty()) {
        QString path = QFileDialog::getSaveFileName(m_mdi->parentWidget(), "Save project",
                                                    qApp->applicationDirPath(), "SDR Files (*.sdr)");
        if(path.isEmpty()) {
            return false;
        }
        return widget->project()->save(path);
    }
    return widget->project()->save();
}

bool MultiWindowView::saveAs()
{
    WindowProject *widget = getActiveWidget();
    if(widget == nullptr) {
        return false;
    }
    QString path = QFileDialog::getSaveFileName(m_mdi->parentWidget(), "Save project as...",
                                                qApp->applicationDirPath(), "SDR Files (*.sdr)");
    if(path.isEmpty()) {
        return false;
    }
    return widget->project()->save(path);
}

WindowProject *MultiWindowView::getActiveWidget()
{
    QMdiSubWindow *wnd = m_mdi->activeSubWindow();
    if(wnd == nullptr) {
        return nullptr;
    }
    WindowProject *widget = qobject_cast<WindowProject*>(wnd->widget());
    if(widget == nullptr) {
        return nullptr;
    }
    return widget;
}

void MultiWindowView::onProjectCreated(QSharedPointer<Project> project)
{
    QLOG_DEBUG() << "Adding new project window";
    WindowProject *widget = new WindowProject(project);
    QMdiSubWindow *wnd = m_mdi->addSubWindow(widget);
    wnd->setWindowTitle(project->path());
    wnd->setWindowIcon(QIcon(":/icons/app/icon.png"));
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->show();
}

void MultiWindowView::onActiveWindowChanged(QMdiSubWindow *wnd)
{
    if(wnd == nullptr) {
        return;
    }
    QLOG_DEBUG() << "Active window changed to " << wnd->windowTitle();
}
