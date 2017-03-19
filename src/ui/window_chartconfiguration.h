#ifndef WINDOW_CHARTCONFIGURATION_H
#define WINDOW_CHARTCONFIGURATION_H

#include <QDialog>
#include <project/chart.h>

namespace Ui {
class WindowChartConfiguration;
}

class WindowChartConfiguration : public QDialog
{
    Q_OBJECT

public:
    WindowChartConfiguration(Chart const& chart, QList<Channel> const& channels, QWidget *parent = 0);
    ~WindowChartConfiguration();

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();

private:
    Ui::WindowChartConfiguration *ui;
    Chart m_chart;
};

#endif // WINDOW_CHARTCONFIGURATION_H
