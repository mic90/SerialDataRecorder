#ifndef PARSERSMANAGER_H
#define PARSERSMANAGER_H

#include <QObject>
#include <plugins/idataparser.h>

class ParsersManager : public QObject
{
    Q_OBJECT
public:
    explicit ParsersManager(QObject *parent = 0);
    ~ParsersManager();

    void loadPlugins();

    QStringList getParsersNames() const;
    QSharedPointer<IDataParserObj> getParserPlugin(const QString& name);
    QSharedPointer<DataParserBase> createParser(const QString& name);

private:
    QList<QSharedPointer<IDataParserObj>> m_parsers;
};

#endif // PARSERSMANAGER_H
