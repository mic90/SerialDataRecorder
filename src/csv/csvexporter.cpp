#include "csvexporter.h"
#include <QsLog.h>
#include <QFile>

bool CsvExporter::exportToFile(const CsvData &data, const QString &path)
{
    QStringList header = data.header();
    if(header.isEmpty())
    {
        QLOG_ERROR() << "Can't export data, no header set";
        return false;
    }
    QFile f(path);
    if(!f.open(QIODevice::WriteOnly))
    {
        QLOG_ERROR() << "Can't export data, can't open file for write";
        return false;
    }
    f.write(header.join(",").append("\n").toUtf8());
    auto headerSize = header.size();
    qint64 written = 0;
    auto const& rawData = data.data();
    for(auto const& jsonArray : rawData)
    {
        QStringList stringified;
        for(int i=0; i<headerSize; i++)
        {
            if(i >= jsonArray.size())
            {
                stringified.append("");
                continue;
            }
            stringified.append(QString::number(jsonArray.at(i).toDouble()));
        }
        written += f.write(stringified.join(",").append("\n").toUtf8());
    }
    QLOG_INFO() << "Write finished, " << written << " bytes written";
    return true;
}
