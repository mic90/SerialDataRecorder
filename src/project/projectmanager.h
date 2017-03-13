#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <project/project.h>

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectManager(QObject *parent = 0);

    bool createProject();
    bool openProject(const QString& path);

    QSharedPointer<Project> getProject(int index);
    QSharedPointer<Project> getProject(const QString& name);

signals:
    void projectCreated(QSharedPointer<Project>);
    void projectOpened(QSharedPointer<Project>);

private:
    QList<QSharedPointer<Project>> m_projects;
};

#endif // PROJECTMANAGER_H
