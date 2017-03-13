#ifndef IDATAPARSER_H
#define IDATAPARSER_H

#include <QtPlugin>

class DataParserBase
{
public:
    virtual QJsonObject parse(const QString&)
    {
        return QJsonObject();
    }
    virtual ~DataParserBase() = default;
};

class IDataParser
{
public:
    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;
    virtual DataParserBase* getParser() const = 0;
};

class IDataParserObj
{
public:
    IDataParserObj(IDataParser* plugin) :
        m_name(plugin->getName()),
        m_description(plugin->getDescription()),
        m_plugin(plugin)
    {
    }

    QString m_name;
    QString m_description;
    IDataParser* m_plugin;
};

QT_BEGIN_NAMESPACE

#define IDataParser_iid "sda.idataparser"
Q_DECLARE_INTERFACE(IDataParser, IDataParser_iid)

QT_END_NAMESPACE

#endif // IDATAPARSER_H
