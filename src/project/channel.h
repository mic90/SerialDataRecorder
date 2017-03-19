#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QVariant>

class Channel
{
public:
    Channel();
    Channel(QString const& name, int id, QVariant const& value = QVariant(0.0));
    Channel(Channel const& other);

    ~Channel() = default;

    QJsonObject toJson() const;
    bool fromJson(QJsonObject const& obj);

    int id() const;
    void setId(int id);
    QString name() const;
    void setName(const QString &name);
    QVariant value() const;
    void setValue(const QVariant &value);

private:
    int m_id;
    QString m_name;
    QVariant m_value;
};

#endif // VARIABLE_H
