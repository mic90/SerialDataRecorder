#include "variablesview.h"
#include <QsLog.h>

VariablesView::VariablesView(QTableWidget *widget, QSharedPointer<Project> project, QObject *parent) :
    QObject(parent),
    m_project(project),
    m_table(widget)
{
    connect(m_project.data(), &Project::variableAdded, this, &VariablesView::onVariableAdded);
    connect(m_project.data(), &Project::variableValueChanged, this, &VariablesView::onVariableValueChanged);
    for(auto variable : m_project->getVariables()) {
        addVariableItem(variable);
    }
}

void VariablesView::addVariableItem(QSharedPointer<Variable> var)
{
    QLOG_DEBUG() << "Adding new variable visual item for" << var->name() << " #" << var->id();
    QTableWidgetItem *nameItem = new QTableWidgetItem(var->name());
    QTableWidgetItem *valueItem = new QTableWidgetItem("");
    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setItem(row, 0, nameItem);
    m_table->setItem(row, 1, valueItem);
    nameItem->setToolTip("Id: " + var->id());

    m_items.insert(var, valueItem);
}

void VariablesView::onVariableAdded(QSharedPointer<Variable> var)
{
    addVariableItem(var);
}

void VariablesView::onVariableValueChanged(QSharedPointer<Variable> var)
{
    m_items.value(var)->setText(var->value().toString());
}
