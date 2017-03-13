#ifndef MULTIWINDOWVIEW_H
#define MULTIWINDOWVIEW_H

#include <QObject>
#include <QMdiArea>
#include <project/project.h>
#include <ui/window_project.h>

class MultiWindowView : public QObject
{
    Q_OBJECT
public:
    MultiWindowView(QMdiArea *mdi, QObject *parent = 0);

    bool save();
    bool saveAs();

private:
    WindowProject *getActiveWidget();

public slots:
    void onProjectCreated(QSharedPointer<Project>);
    void onActiveWindowChanged(QMdiSubWindow*);

private:
    QMdiArea *m_mdi;
};

#endif // MULTIWINDOWVIEW_H
