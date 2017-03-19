#ifndef VARIABLESTABLEMODEL_H
#define VARIABLESTABLEMODEL_H

#include <memory>
#include <QObject>
#include <QAbstractTableModel>
#include <QSharedPointer>
#include <project/channel.h>

class VariablesTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(VariablesTableModel)
public:
    VariablesTableModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setVariables(QList<Channel> const& varList);

public slots:
    void onVariableAdded(Channel const& variable);

private:
    QList<Channel> m_variables;
};

#endif // VARIABLESTABLEMODEL_H
