#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include <QString>
#include <csv/csvdata.h>

class CsvExporter
{
public:
    static bool exportToFile(const CsvData &data, const QString &path);
};

#endif // CSVEXPORTER_H
