#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QSharedPointer>

#include <serial/serialportconfig.h>
#include <project/chart.h>
#include <project/variable.h>
#include <plugins/idataparser.h>

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(const QString& path = "", QObject *parent = 0);
    ~Project();

    bool load(const QString& path);
    bool load();
    bool save(const QString& path);
    bool save();

    QString path() const;
    void setPath(const QString &path);
    QString name() const;
    SerialPortConfig serialConfig() const;
    void setSerialConfig(const SerialPortConfig &serialConfig);
    QString getDataParserName() const;
    void setDataParserName(const QString &dataParserName);

    QSharedPointer<Variable> getVariable(const QString &name, const QString &id = "");
    void addVariable(QSharedPointer<Variable>);
    void addVariable(const QString& name, const QString& id = "");
    QList<QSharedPointer<Variable>> getVariables();

public slots:
    void setVariableValue(const QJsonObject& obj);

signals:
    void pathChanged(const QString& path);
    void nameChanged(const QString& path);
    void variableAdded(QSharedPointer<Variable>);
    void variableChanged(QSharedPointer<Variable>);
    void variableValueChanged(QSharedPointer<Variable>);
    void variableRemoved(QSharedPointer<Variable>);

private:
    QByteArray serialize();
    bool deserialize(const QByteArray& data);

private:
    QString m_path;
    QString m_name;
    SerialPortConfig m_serialConfig;
    QList<QSharedPointer<Variable>> m_variables;
    QList<QSharedPointer<Chart>> m_charts;
    QString m_dataParserName;
};

#endif // PROJECT_H
