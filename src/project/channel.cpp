#include "channel.h"
#include <QJsonObject>

Channel::Channel() : Channel("", -1)
{
}

Channel::Channel(const QString &name, int id):
    m_id(id),
    m_name(name)
{
}

Channel::Channel(const Channel &other)
{
    m_id = other.m_id;
    m_name = other.m_name;
}

bool Channel::operator ==(const Channel &other)
{
    return m_id == other.m_id &&
            m_name == other.m_name;
}

QJsonObject Channel::toJson() const
{
    QJsonObject channelObj
    {
        {"name", m_name},
        {"id", m_id}
    };
    return channelObj;
}

bool Channel::fromJson(QJsonObject const& obj)
{
    if(!obj.contains("name") || !obj.contains("id"))
    {
        return false;
    }
    m_name = obj.value("name").toString();
    m_id = obj.value("id").toInt();
    return true;
}

int Channel::id() const
{
    return m_id;
}

void Channel::setId(int id)
{
    m_id = id;
}

QString Channel::name() const
{
    return m_name;
}

void Channel::setName(const QString &name)
{
    m_name = name;
}
