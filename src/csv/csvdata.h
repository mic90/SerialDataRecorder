#ifndef CSVDATA_H
#define CSVDATA_H

#include <QObject>
#include <QStringList>
#include <QJsonArray>
#include <project/channel.h>

class CsvData : public QObject
{
    Q_OBJECT
public:
    explicit CsvData(QObject *parent = nullptr);

    void setHeader(const QStringList &header);
    void setHeader(const QList<Channel> &channels);
    void clear();
    int dataSize();

    QStringList header() const;
    const QList<QJsonArray>& data() const;

public slots:
    void addData(const QList<QJsonArray> &data);

private:
    QStringList m_header;
    QList<QJsonArray> m_data;
};

#endif // CSVDATA_H
