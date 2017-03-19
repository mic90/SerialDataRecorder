#include "ui/chartwidget.h"
#include "ui_chartwidget.h"
#include <QWheelEvent>
#include <QInputDialog>
#include <QsLog.h>

const int MIN_HEIGHT = 150;
const int MIN_XRANGE = 100;
const int RESIZE_RATIO = 20;
const int XRESIZE_RATION = 100;
const QColor PRESSED_COLOR = QColor(102, 187, 106);
const QColor NORMAL_COLOR = QColor(230, 230, 230);

ChartWidget::ChartWidget(Chart const& chart, QList<Channel> const& channels, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartWidget),
    m_channels(channels),
    m_chart(chart),
    m_dataCount(0),
    m_pause(false)
{
    ui->setupUi(this);
    setMinimumHeight(chart.minimumHeight());

    setChart(m_chart);

    m_plot.legend->setVisible(true);
    m_plot.legend->setFont(QFont("Helvetica", 9));
    m_plot.axisRect(0)->setRangeDrag(Qt::Horizontal);
    m_plot.axisRect(0)->setRangeZoom(Qt::Horizontal);
    m_plot.setInteraction(QCP::iRangeDrag, true);
    m_plot.setInteraction(QCP::iRangeZoom, true);
    m_plot.replot();
    ui->horizontalLayout->addWidget(&m_plot);

    m_plot.installEventFilter(this);

    m_colorAnimation.reset(new QPropertyAnimation(this, "color"));
    m_colorAnimation->setDuration(250);
    m_colorAnimation->setEasingCurve(QEasingCurve::InCubic);
    m_colorAnimation->setEndValue(PRESSED_COLOR);
    m_colorAnimation->setStartValue(NORMAL_COLOR);

    ui->frame->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                            .arg(NORMAL_COLOR.red()).arg(NORMAL_COLOR.green()).arg(NORMAL_COLOR.blue()));
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

bool ChartWidget::exportImage(const QString &fileName)
{
//    auto range = m_plot.xAxis->range();
//    m_plot.xAxis->setRange(0, m_dataCount);
//    m_plot.replot();
    auto ok = m_plot.savePng(fileName, m_plot.width(), m_plot.height(), 1.0, 100);
//    m_plot.xAxis->setRange(range);
//    m_plot.replot();
    return ok;
}

void ChartWidget::clearData()
{
    for(auto graph : m_graphs)
    {
        graph->clearData();
    }
    m_plot.xAxis->setRange(0, m_chart.samplesCount());
    m_plot.replot();
    m_dataCount = 0;
}

void ChartWidget::setPause(bool enabled)
{
    m_pause = enabled;
}

QColor ChartWidget::color() const
{
    return m_color;
}

void ChartWidget::setChart(const Chart &chart)
{
    m_plot.clearGraphs();
    m_graphs.clear();

    // create graphs and assign data to it:
    for(auto channelId : chart.channels())
    {
        if(channelId > m_channels.size())
        {
            continue;
        }
        auto *graph = m_plot.addGraph();
        graph->setName(m_channels.at(channelId).name());
        graph->setAntialiased(true);
        m_graphs.append(graph);
    }

    m_plot.xAxis->setLabel(chart.xAxis());
    m_plot.yAxis->setLabel(chart.yAxis());
    m_plot.xAxis->setRange(m_dataCount, chart.samplesCount(), Qt::AlignRight);
    if(chart.yAxisAutorange())
    {
        m_plot.yAxis->setRange(0, 1);
    }
    else
    {
        m_plot.yAxis->setRange(chart.yAxisMin(), chart.yAxisMax());
    }
    m_plot.replot();

    m_chart = chart;
}

void ChartWidget::setChannels(const QList<Channel> &channels)
{
    m_channels.clear();
    m_channels.append(channels);
    setChart(m_chart);
}

void ChartWidget::setColor(QColor color)
{
    m_color = color;
    ui->frame->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                             .arg(color.red()).arg(color.green()).arg(color.blue()));
}

void ChartWidget::setData(QJsonArray data)
{
    if(m_pause)
    {
        return;
    }

    auto ids = m_chart.channels();
    for(int i=0; i<ids.size(); i++)
    {
        if(ids.at(i) > data.size())
        {
            continue;
        }
        m_graphs[i]->addData(m_dataCount, data.at(ids.at(i)).toDouble());
    }
    if(m_chart.yAxisAutorange())
    {
        for(auto graph : m_graphs)
        {
            graph->rescaleValueAxis();
        }
    }
    m_plot.xAxis->setRange(m_dataCount, m_chart.samplesCount(), Qt::AlignRight);
    m_plot.replot();
    m_dataCount++;
}

void ChartWidget::mouseDoubleClickEvent(QMouseEvent *)
{
}

bool ChartWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Wheel)
    {
        QWheelEvent *ev = static_cast<QWheelEvent*>(event);
        if(QGuiApplication::queryKeyboardModifiers() & Qt::ControlModifier)
        {
            auto delta = ev->angleDelta().y();
            if(delta > 0)
            {
                setMinimumHeight(height() + RESIZE_RATIO);
            }
            else if(height() > MIN_HEIGHT)
            {
                setMinimumHeight(height() - RESIZE_RATIO);
            }
            if(height() < MIN_HEIGHT)
            {
                setMinimumHeight(MIN_HEIGHT);
            }
            emit minimumHeightChanged(minimumHeight());
            return true;
        }
        else if(QGuiApplication::queryKeyboardModifiers() & Qt::ShiftModifier)
        {
            auto delta = ev->angleDelta().y();
            if(delta > 0)
            {
                m_chart.setSamplesCount(m_chart.samplesCount() + XRESIZE_RATION);
            }
            else if(height() > MIN_HEIGHT)
            {
                m_chart.setSamplesCount(m_chart.samplesCount() - XRESIZE_RATION);
            }
            if(m_chart.samplesCount() < MIN_XRANGE)
            {
                m_chart.setSamplesCount(MIN_XRANGE);
            }
            m_plot.xAxis->setRange(m_dataCount, m_chart.samplesCount(), Qt::AlignRight);
            m_plot.replot();
            return true;
        }
        else
        {
            QApplication::sendEvent(parent(), ev);
            return true;
        }
    }
    else if(event->type() == QEvent::MouseButtonDblClick)
    {
        emit editRequested();
    }
    return QObject::eventFilter(watched, event);
}


void ChartWidget::enterEvent(QEvent *)
{
    m_colorAnimation->start();
}

void ChartWidget::leaveEvent(QEvent *)
{
    m_colorAnimation->stop();
    ui->frame->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                            .arg(NORMAL_COLOR.red()).arg(NORMAL_COLOR.green()).arg(NORMAL_COLOR.blue()));
}
