#include "variablestablemodel.h"
#include <QDebug>

VariablesTableModel::VariablesTableModel(QObject *parent): QAbstractTableModel(parent)
{

}

int VariablesTableModel::rowCount(const QModelIndex &) const
{
    return m_variables.size();
}

int VariablesTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant VariablesTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return m_variables.at(index.row()).name();
        }
        if(index.column() == 1)
        {
            auto const& variable = m_variables.at(index.row());
            return variable.value().toString();
        }
    }
    else if(role == Qt::UserRole)
    {
        return m_variables.at(index.row()).id();
    }
    return QVariant();
}

QVariant VariablesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return "Name";
        }
        if(section == 1)
        {
            return "Value";
        }
    }
    return QVariant();
}

void VariablesTableModel::setVariables(const QList<Channel> &varList)
{
    beginResetModel();

    m_variables.clear();
    m_variables.append(varList);
    endResetModel();
}

void VariablesTableModel::onVariableAdded(Channel const& variable)
{
    beginInsertRows(QModelIndex(), m_variables.size(), m_variables.size());
    m_variables.append(variable);
    endInsertRows();
}
