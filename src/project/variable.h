#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include <QVariant>

class Variable : public QObject
{
    Q_OBJECT
public:
    Variable(const QString& name, const QString &id = "", const QVariant& value = QVariant() , QObject *parent = 0);
    ~Variable();

    QString id() const;
    void setId(const QString &id);
    QString name() const;
    void setName(const QString &name);
    QVariant value() const;
    void setValue(const QVariant &value);

private:
    QString m_id;
    QString m_name;
    QVariant m_value;
};

#endif // VARIABLE_H
