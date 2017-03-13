#ifndef VARIABLEITEM_H
#define VARIABLEITEM_H

#include <QObject>
#include <QTableWidgetItem>

#include <project/variable.h>

class VariableItem : public QObject
{
    Q_OBJECT
public:
    explicit VariableItem(QObject *parent = 0);

private:
    QSharedPointer<Variable> m_var;
    QSharedPointer<QTableWidgetItem> m_nameItem;
    QSharedPointer<QTableWidgetItem> m_valueItem;
};

#endif // VARIABLEITEM_H
