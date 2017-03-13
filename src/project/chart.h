#ifndef CHART_H
#define CHART_H

#include <QObject>

class Chart : public QObject
{
    Q_OBJECT
public:
    explicit Chart(QObject *parent = 0);
    ~Chart();
};

#endif // CHART_H
