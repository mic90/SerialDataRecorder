#include "variable.h"

Variable::Variable(const QString &name, const QString &id, const QVariant &value, QObject *parent) : QObject(parent),
    m_id(id),
    m_name(name),
    m_value(value)
{

}

Variable::~Variable()
{

}

QString Variable::id() const
{
    return m_id;
}

void Variable::setId(const QString &id)
{
    m_id = id;
}

QString Variable::name() const
{
    return m_name;
}

void Variable::setName(const QString &name)
{
    m_name = name;
}

QVariant Variable::value() const
{
    return m_value;
}

void Variable::setValue(const QVariant &value)
{
    m_value = value;
}
