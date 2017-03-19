#ifndef PARSERSMANAGER_H
#define PARSERSMANAGER_H

#include <QObject>
#include <memory>
#include <parsers/dataparser.h>

class DataParserFactory : public QObject
{
    Q_OBJECT
public:
    explicit DataParserFactory(QObject *parent = 0);
    ~DataParserFactory();

    QStringList getParsersNames() const;
    std::unique_ptr<DataParserBase> createParser(const QString& name);

private:
    std::vector<std::unique_ptr<DataParserPluginObj>> m_parsers;
};

#endif // PARSERSMANAGER_H
