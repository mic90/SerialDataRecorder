#ifndef SDRDATAPARSER_H
#define SDRDATAPARSER_H

#include <plugins/idataparser.h>

class SdrDataParser : public DataParserBase
{
public:
    virtual QJsonObject parse(const QString &data) override;
    virtual ~SdrDataParser();

private:
    QString m_tempData;
};

class SdrDataParserPlugin : public IDataParser
{
    Q_INTERFACES(IDataParser)
public:
    QString getName() const { return "SerialDataRecorder"; }
    QString getDescription() const { return "Default data parser for SDR simple serial protocol"; }
    DataParserBase* getParser() const { return new SdrDataParser(); }
};

#endif // SDRDATAPARSER_H
