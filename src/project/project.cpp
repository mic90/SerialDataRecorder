#include "project.h"
#include <QFile>
#include <QsLog.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

Project::Project(const QString &path, QObject *parent) : QObject(parent),
    m_path(path),
    m_name(""),
    m_serialConfig(),
    m_dataParserName("")
{

}

Project::~Project()
{

}

bool Project::load(const QString &path)
{
    setPath(path);
    return load();
}

bool Project::load()
{
    QLOG_INFO() << "Loading project file" << m_path;
    QFile file(m_path);
    if(!file.exists() || !file.open(QIODevice::ReadOnly)) {
        QLOG_ERROR() << "Can't open file at" << m_path;
        return false;
    }
    bool ret = deserialize(file.readAll());
    file.close();
    return ret;
}

bool Project::save(const QString &path)
{
    setPath(path);
    return save();
}

bool Project::save()
{
    QLOG_INFO() << "Saving project to" << m_path << "...";
    QFile f(m_path);
    if(!f.open(QIODevice::WriteOnly)) {
        QLOG_ERROR() << "Can't open file" << m_path << "for saving";
        return false;
    }
    f.write(serialize());
    f.close();
    return true;
}

QString Project::path() const
{
    return m_path;
}

void Project::setPath(const QString &path)
{
    m_path = path;
    QLOG_DEBUG() << "Path changed to" << m_path;
    QFileInfo fInfo(path);
    m_name = fInfo.baseName();
    emit pathChanged(m_path);
}

QString Project::name() const
{
    return m_name;
}

SerialPortConfig Project::serialConfig() const
{
    return m_serialConfig;
}

void Project::setSerialConfig(const SerialPortConfig &serialConfig)
{
    m_serialConfig = serialConfig;
    if(!m_path.isEmpty()) {
        save();
    }
}

QSharedPointer<Variable> Project::getVariable(const QString &name, const QString &id)
{
    for(int i=0; i<m_variables.size(); i++) {
        if(m_variables.at(i)->name() == name) {
            if(!id.isEmpty() && m_variables.at(i)->id() == id) {
                return m_variables[i];
            } else if(id.isEmpty()){
                return m_variables[i];
            }
        }
    }
    return QSharedPointer<Variable>(nullptr);
}

void Project::addVariable(QSharedPointer<Variable> var)
{
    m_variables.append(var);
    if(var->id().isEmpty()) {
        var->setId(QString::number(m_variables.size()));
    }
    QLOG_INFO() << "Added new variable" << var->name() << "id:" << var->id();
    save();
    emit variableAdded(var);
}

void Project::addVariable(const QString &name, const QString &id)
{
    auto newVar = QSharedPointer<Variable>(new Variable(name, id));
    addVariable(newVar);
}

QList<QSharedPointer<Variable> > Project::getVariables()
{
    return m_variables;
}

void Project::setVariableValue(const QJsonObject &obj)
{
    QString id = obj.value("id").toString();
    QString value = obj.value("value").toString();
    for(auto variable : m_variables) {
        if(variable->id() == id) {
            variable->setValue(value);
            QLOG_DEBUG() << "Variable" << variable->name() << "value changed to" << value;
            emit variableValueChanged(variable);
            return;
        }
    }
}

QByteArray Project::serialize()
{
    QJsonDocument document;
    QJsonObject root;
    root.insert("name", m_name);
    root.insert("parser", m_dataParserName);
    root.insert("serial", m_serialConfig.serialize());
    QJsonArray variables;
    for(auto variable : m_variables) {
        QJsonObject jsonVar;
        jsonVar.insert("name", variable->name());
        jsonVar.insert("id", variable->id());
        variables.append(jsonVar);
    }
    root.insert("variables", variables);
    document.setObject(root);
    return document.toJson();
}

bool Project::deserialize(const QByteArray &data)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject root = document.object();
    if(root.isEmpty()) {
        QLOG_ERROR() << "Root object is empty";
        return false;
    }
    m_dataParserName = root.value("parser").toString();
    QJsonObject serialConfig = root.value("serial").toObject();
    m_serialConfig.deserialize(serialConfig);
    QJsonArray variables = root.value("variables").toArray();
    for(auto jsonVar : variables) {
        QString name = jsonVar.toObject().value("name").toString();
        QString id = jsonVar.toObject().value("id").toString();
        QSharedPointer<Variable> newVar(new Variable(name, id));
        m_variables.append(newVar);
    }
    return true;
}

QString Project::getDataParserName() const
{
    return m_dataParserName;
}

void Project::setDataParserName(const QString &dataParserName)
{
    m_dataParserName = dataParserName;
    if(!m_path.isEmpty()) {
        save();
    }
}
