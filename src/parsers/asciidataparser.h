#ifndef ASCIIDATAPARSER_H
#define ASCIIDATAPARSER_H

#include <parsers/dataparser.h>
#include <memory>

class AsciiDataParser : public DataParserBase
{
public:
    AsciiDataParser();
    virtual QList<QJsonArray> parse(const QString &) override;

private:
    QString m_buffer;
};

class AsciiDataParserPlugin : public DataParserPlugin
{
    Q_INTERFACES(DataParserPlugin)
public:
    QString getName() const { return "ASCII"; }
    QString getDescription() const { return "Simple ASCII based data parser"; }
    std::unique_ptr<DataParserBase> getParser() const { return std::make_unique<AsciiDataParser>(); }
};

#endif // ASCIIDATAPARSER_H
