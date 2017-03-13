#ifndef VARIABLESVIEW_H
#define VARIABLESVIEW_H

#include <QObject>
#include <QTableWidget>
#include <project/project.h>

class VariablesView : public QObject
{
    Q_OBJECT
public:
    VariablesView(QTableWidget *widget, QSharedPointer<Project> project, QObject *parent = 0);

private:
    void addVariableItem(QSharedPointer<Variable> var);

private slots:
    void onVariableAdded(QSharedPointer<Variable> var);
    void onVariableValueChanged(QSharedPointer<Variable> var);

private:
    QSharedPointer<Project> m_project;
    QTableWidget *m_table;
    QHash<QSharedPointer<Variable>, QTableWidgetItem*> m_items;
};

#endif // VARIABLESVIEW_H
