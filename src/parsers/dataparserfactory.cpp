#include "dataparserfactory.h"
#include <parsers/asciidataparser.h>
#include <QsLog.h>

DataParserFactory::DataParserFactory(QObject *parent) : QObject(parent)
{
    m_parsers.push_back(std::move(std::make_unique<DataParserPluginObj>(new AsciiDataParserPlugin())));
    QLOG_INFO() << "Loaded" << m_parsers.size() << "data parsers plugins";
}

DataParserFactory::~DataParserFactory()
{
}

QStringList DataParserFactory::getParsersNames() const
{
    QStringList names;
    for(auto const& parser : m_parsers)
    {
        names.append(parser->m_name);
    }
    return names;
}

std::unique_ptr<DataParserBase> DataParserFactory::createParser(const QString &name)
{
    for(auto const& parser : m_parsers)
    {
        if(parser->m_name == name)
        {
            return std::unique_ptr<DataParserBase>(parser->m_plugin->getParser());
        }
    }
    return nullptr;
}
