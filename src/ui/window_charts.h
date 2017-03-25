#ifndef WINDOW_CHARTS_H
#define WINDOW_CHARTS_H

#include <QDialog>
#include <QAbstractListModel>
#include <project/chart.h>

namespace Ui {
class WindowCharts;
}

class ChartsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ChartsListModel(QObject *parent = nullptr);
    ~ChartsListModel() = default;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    QList<Chart> charts() const;
    void setCharts(const QList<Chart> &charts);

    void addChart();
    void removeChart(QModelIndex const& index);

    void setTitle(QModelIndex const& index, QString const& title);

    Chart& getChart(QModelIndex const& index);

private:
    QList<Chart> m_charts;
};

class WindowCharts : public QDialog
{
    Q_OBJECT

public:
    WindowCharts(QList<Chart> charts = QList<Chart>(),
                 QList<Channel> channels = QList<Channel>(),
                 QWidget *parent = 0);
    ~WindowCharts();

    QList<Chart> getCharts() const;

private slots:
    void onTitleChartChanged(QString const& title);
    void onXAxisChanged(QString const& name);
    void onYAxisChanged(QString const& name);
    void onYAxisAutorangeChanged(bool enabled);
    void onYAxisMinChanged(double value);
    void onYAxisMaxChanged(double value);
    void onXAxisRangeChanged(int value);
    void onChannelsChanged(QList<int> const& channels);

    void on_ok_clicked();
    void on_cancel_clicked();
    void on_charts_clicked(const QModelIndex &index);
    void on_addChart_clicked();
    void on_removeChart_clicked();

private:
    Ui::WindowCharts *ui;
    ChartsListModel m_chartsModel;
};

#endif // WINDOW_CHARTS_H
