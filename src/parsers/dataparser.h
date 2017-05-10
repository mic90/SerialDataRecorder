#ifndef IDATAPARSER_H
#define IDATAPARSER_H

#include <QtPlugin>
#include <memory>
#include <QJsonArray>

class DataParserBase
{
    Q_DISABLE_COPY(DataParserBase)
public:
    DataParserBase() = default;
    ~DataParserBase() = default;

    virtual QList<QJsonArray> parse(const QString&) = 0;
};

class DataParserPlugin
{
public:
    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;
    virtual QString getExampleFormat() const = 0;
    virtual std::unique_ptr<DataParserBase> getParser() const = 0;
};

class DataParserPluginObj
{
public:
    DataParserPluginObj(DataParserPlugin* plugin) :
        m_name(plugin->getName()),
        m_description(plugin->getDescription()),
        m_plugin(plugin)
    {
    }

    QString m_name;
    QString m_description;
    DataParserPlugin* m_plugin;
};

QT_BEGIN_NAMESPACE

#define IDataParser_iid "sdr.idataparser"
Q_DECLARE_INTERFACE(DataParserPlugin, IDataParser_iid)

QT_END_NAMESPACE

#endif // IDATAPARSER_H
