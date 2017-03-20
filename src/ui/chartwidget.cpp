#include "ui/chartwidget.h"
#include "ui_chartwidget.h"
#include <QWheelEvent>
#include <QInputDialog>
#include <QsLog.h>

const int MIN_HEIGHT = 150;
const int MIN_XRANGE = 100;
const int YRESIZE_RATIO = 20;
const int XRESIZE_RATIO = 100;
const QColor PRESSED_COLOR = QColor(102, 187, 106);
const QColor NORMAL_COLOR = QColor(230, 230, 230);

const int EXPORT_IMG_QUALITY = 100;
const int EXPORT_IMG_SIZE_FACTOR = 1;
const int CHART_REFRESH_INTERVAL = 50;

ChartWidget::ChartWidget(Chart const& chart, QList<Channel> const& channels, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartWidget),
    m_title(new QCPTextElement(&m_plot, chart.name(), QFont("sans", 11))),
    m_channels(channels),
    m_chart(chart),
    m_dataCount(0),
    m_pause(false)
{
    ui->setupUi(this);
    setMinimumHeight(chart.minimumHeight());

    setChart(m_chart);

    //disable antialiasing
    setAntialiasing(false);

    //add chart title
    m_plot.plotLayout()->insertRow(0);
    m_plot.plotLayout()->addElement(0, 0, m_title);

    m_plot.legend->setVisible(true);
    m_plot.axisRect()->setRangeDrag(Qt::Horizontal);
    m_plot.setInteraction(QCP::iRangeDrag, true);
    setLightTheme();
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

    m_refreshTimer.setInterval(CHART_REFRESH_INTERVAL);
    connect(&m_refreshTimer, SIGNAL(timeout()), this, SLOT(refreshChart()));
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

bool ChartWidget::exportImage(const QString &filePath)
{
    QLOG_INFO() << "Saving chart image to" << filePath;
    setAntialiasing(true);
    m_plot.replot();
    bool ok = m_plot.savePng(filePath, m_plot.width()*EXPORT_IMG_SIZE_FACTOR,
                          m_plot.height()*EXPORT_IMG_SIZE_FACTOR, EXPORT_IMG_SIZE_FACTOR, EXPORT_IMG_QUALITY);
    setAntialiasing(false);
    m_plot.replot();
    return ok;
}

void ChartWidget::clearData()
{
    for(auto graph : m_graphs)
    {
        graph->setData(QVector<double>(), QVector<double>());
    }
    m_plot.xAxis->setRange(0, m_chart.samplesCount());
    m_plot.replot();
    m_dataCount = 0;
}

void ChartWidget::setPause(bool enabled)
{
    m_pause = enabled;
}

void ChartWidget::setDarkTheme()
{
    m_plot.xAxis->setBasePen(QPen(Qt::white, 1));
    m_plot.yAxis->setBasePen(QPen(Qt::white, 1));
    m_plot.xAxis->setTickPen(QPen(Qt::white, 1));
    m_plot.yAxis->setTickPen(QPen(Qt::white, 1));
    m_plot.xAxis->setSubTickPen(QPen(Qt::white, 1));
    m_plot.yAxis->setSubTickPen(QPen(Qt::white, 1));
    m_plot.xAxis->setTickLabelColor(Qt::white);
    m_plot.yAxis->setTickLabelColor(Qt::white);
    m_plot.xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    m_plot.yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    m_plot.xAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_plot.yAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_plot.xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_plot.yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_plot.legend->setBrush(QBrush(QColor(20, 20, 20)));
    m_plot.legend->setTextColor(Qt::white);
    m_title->setTextColor(Qt::white);

    m_plot.setBackground(QBrush(QColor(20, 20, 20)));
    m_plot.axisRect()->setBackground(QBrush(QColor(20, 20, 20)));

    m_plot.replot();
}

void ChartWidget::setLightTheme()
{
    m_plot.xAxis->setBasePen(QPen(Qt::black, 1));
    m_plot.yAxis->setBasePen(QPen(Qt::black, 1));
    m_plot.xAxis->setTickPen(QPen(Qt::black, 1));
    m_plot.yAxis->setTickPen(QPen(Qt::black, 1));
    m_plot.xAxis->setSubTickPen(QPen(Qt::black, 1));
    m_plot.yAxis->setSubTickPen(QPen(Qt::black, 1));
    m_plot.xAxis->setTickLabelColor(Qt::black);
    m_plot.yAxis->setTickLabelColor(Qt::black);
    m_plot.xAxis->grid()->setPen(QPen(QColor(70, 70, 70), 1, Qt::DotLine));
    m_plot.yAxis->grid()->setPen(QPen(QColor(70, 70, 70), 1, Qt::DotLine));
    m_plot.xAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_plot.yAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_plot.xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_plot.yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_plot.legend->setBrush(QBrush(Qt::white));
    m_plot.legend->setTextColor(Qt::black);
    m_title->setTextColor(Qt::black);

    m_plot.setBackground(QBrush(Qt::white));
    m_plot.axisRect()->setBackground(QBrush(Qt::white));

    m_plot.replot();
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
    m_title->setText(chart.name());
    m_plot.replot();
    m_chart = chart;
    m_refreshTimer.start();
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
    m_dataCount++;
}

void ChartWidget::refreshChart()
{
    m_plot.replot();
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
                setMinimumHeight(height() + YRESIZE_RATIO);
            }
            else if(height() > MIN_HEIGHT)
            {
                setMinimumHeight(height() - YRESIZE_RATIO);
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
                m_chart.setSamplesCount(m_chart.samplesCount() + XRESIZE_RATIO);
            }
            else if(height() > MIN_HEIGHT)
            {
                m_chart.setSamplesCount(m_chart.samplesCount() - XRESIZE_RATIO);
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

void ChartWidget::setAntialiasing(bool enabled)
{
    QFont font;
    if(enabled)
    {
        m_plot.setAntialiasedElements(QCP::aeAll);
        font.setStyleStrategy(QFont::PreferAntialias);
    }
    else
    {
        m_plot.setNotAntialiasedElements(QCP::aeAll);
        QFont font;
        font.setStyleStrategy(QFont::NoAntialias);
    }
    for(auto graph : m_graphs)
    {
        graph->setAntialiased(enabled);
    }
    m_plot.xAxis->setTickLabelFont(font);
    m_plot.yAxis->setTickLabelFont(font);
    m_plot.legend->setFont(font);
}
