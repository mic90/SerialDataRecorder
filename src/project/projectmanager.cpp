#include "projectmanager.h"
#include <QsLog.h>

ProjectManager::ProjectManager(QObject *parent) : QObject(parent)
{

}

bool ProjectManager::createProject()
{
    QSharedPointer<Project> newProject(new Project());
    m_projects.append(newProject);
    QLOG_INFO() << "New project created, projects count: " << m_projects.size();
    emit projectCreated(newProject);
    return true;
}

bool ProjectManager::openProject(const QString &path)
{
    QSharedPointer<Project> newProject(new Project(path));
    bool ret = newProject->load();
    if(ret) {
        m_projects.append(newProject);
        emit projectOpened(newProject);
        return true;
    }
    return false;
}

QSharedPointer<Project> ProjectManager::getProject(int index)
{

}

QSharedPointer<Project> ProjectManager::getProject(const QString &name)
{

}
