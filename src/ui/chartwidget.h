#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <external/qcustomplot/qcustomplot.h>
#include <project/chart.h>

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ChartWidget)
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    ChartWidget(Chart const& chart, QList<Channel> const& channels, QWidget *parent = 0);
    ~ChartWidget();

    void exportImage(QString const& filePath);

    void clearData();
    void setPause(bool enabled);

    void setDarkTheme();
    void setLightTheme();

    QColor color() const;
    void setChart(Chart const& chart);
    void setChannels(QList<Channel> const& channels);

public slots:
    void setColor(QColor color);
    void setData(QList<QJsonArray> data);

private slots:
    void refreshChart();

signals:
    void minimumHeightChanged(int width);
    void editRequested();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    void setAntialiasing(bool enabled);

private:
    Ui::ChartWidget *ui;
    QCustomPlot m_plot;
    QColor m_color;
    QScopedPointer<QPropertyAnimation> m_colorAnimation;
    QList<QCPGraph*> m_graphs;
    QCPTextElement* m_title;
    QList<Channel> m_channels;
    Chart m_chart;
    long long m_dataCount;
    bool m_pause;
    QTimer m_refreshTimer;
    QList<QJsonArray> m_buffer;
    QMutex m_mutex;
    QList<QColor> m_graphColors;
};

#endif // CHARTWIDGET_H
