#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QVariant>

class Channel
{
public:
    Channel();
    Channel(QString const& name, int id);
    Channel(Channel const& other);
    ~Channel() = default;

    bool operator ==(Channel const& other);

    QJsonObject toJson() const;
    bool fromJson(QJsonObject const& obj);

    int id() const;
    void setId(int id);
    QString name() const;
    void setName(const QString &name);

private:
    int m_id;
    QString m_name;
};

#endif // VARIABLE_H
