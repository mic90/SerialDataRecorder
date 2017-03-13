#ifndef MYSENSORSDATAPARSER_H
#define MYSENSORSDATAPARSER_H

#include <plugins/idataparser.h>

class MySensorsDataParser : public DataParserBase
{
public:
    virtual QJsonObject parse(const QString &data) override;
    virtual ~MySensorsDataParser();

private:
    QString m_tempData;
};

class MySensorsDataParserPlugin : public IDataParser
{
    Q_INTERFACES(IDataParser)
public:
    QString getName() const { return "MySensors"; }
    QString getDescription() const { return "Data parser for MySensors serial gateway"; }
    DataParserBase* getParser() const { return new MySensorsDataParser(); }
};

#endif // MYSENSORSDATAPARSER_H
