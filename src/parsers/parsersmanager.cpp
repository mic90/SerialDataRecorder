#include "parsersmanager.h"
#include <parsers/mysensorsdataparser.h>
#include <parsers/sdrdataparser.h>
#include <QsLog.h>

ParsersManager::ParsersManager(QObject *parent) : QObject(parent)
{
}

ParsersManager::~ParsersManager()
{
}

void ParsersManager::loadPlugins()
{
    QSharedPointer<IDataParserObj> mysensorsParser(new IDataParserObj(new MySensorsDataParserPlugin()));
    m_parsers.append(mysensorsParser);
    QSharedPointer<IDataParserObj> sdrParser(new IDataParserObj(new SdrDataParserPlugin()));
    m_parsers.append(sdrParser);

    QLOG_INFO() << "Loaded" << m_parsers.size() << "data parsers plugins";
}

QStringList ParsersManager::getParsersNames() const
{
    QStringList names;
    for(auto parser : m_parsers) {
        names.append(parser->m_name);
    }
    return names;
}

QSharedPointer<IDataParserObj> ParsersManager::getParserPlugin(const QString &name)
{
    for(int i=0; i<m_parsers.size(); i++) {
        if(m_parsers.at(i)->m_name == name) {
            return m_parsers.at(i);
        }
    }
    return QSharedPointer<IDataParserObj>(nullptr);
}

QSharedPointer<DataParserBase> ParsersManager::createParser(const QString &name)
{
    for(int i=0; i<m_parsers.size(); i++) {
        if(m_parsers.at(i)->m_name == name) {
            return QSharedPointer<DataParserBase>(m_parsers.at(i)->m_plugin->getParser());
        }
    }
    return QSharedPointer<DataParserBase>(nullptr);
}
