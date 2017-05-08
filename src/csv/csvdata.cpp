#include "csvdata.h"

const int MAX_DATA_COUNT = 1000000;

CsvData::CsvData(QObject *parent) : QObject(parent)
{

}

void CsvData::setHeader(const QStringList &header)
{
    m_header = header;
}

void CsvData::setHeader(const QList<Channel> &channels)
{
    for(auto const& channel : channels)
    {
        m_header.append(channel.name());
    }
}

void CsvData::addData(const QList<QJsonArray> &data)
{
    if(m_data.size() >= MAX_DATA_COUNT) {
        m_data.removeFirst();
    }
    m_data.append(data);
}

const QList<QJsonArray>& CsvData::data() const
{
    return m_data;
}

QStringList CsvData::header() const
{
    return m_header;
}

void CsvData::clear()
{
    m_data.clear();
}

int CsvData::dataSize()
{
    return m_data.size();
}
