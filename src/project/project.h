#ifndef PROJECT_H
#define PROJECT_H

#include <memory>
#include <QObject>
#include <QSharedPointer>

#include <serial/serialportconfig.h>
#include <project/chart.h>
#include <project/channel.h>

class Project : public QObject
{
    Q_OBJECT
public:
    Project(const QString& path = "", QObject *parent = 0);
    ~Project() = default;

    bool load(const QString& path);
    bool load();
    bool save(const QString& path);
    bool save();

    QString path() const;
    void setPath(const QString &path);
    QString name() const;
    SerialPortConfig serialConfig() const;
    void setSerialConfig(const SerialPortConfig &serialConfig);
    QString getDataParserName() const;
    void setDataParserName(const QString &dataParserName);

    QList<Channel> getChannels() const;
    void setChannels(QList<Channel> const& channels);

    QList<Chart> getCharts() const;
    void setCharts(QList<Chart> const& charts);

signals:
    void pathChanged(const QString& path);
    void nameChanged(const QString& path);

private:
    QByteArray toJson();
    bool fromJson(const QByteArray& data);

private:
    QString m_path;
    QString m_name;
    SerialPortConfig m_serialConfig;
    QList<Channel> m_channels;
    QList<Chart> m_charts;
    QString m_dataParserName;
};

#endif // PROJECT_H
