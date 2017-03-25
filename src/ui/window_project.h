#ifndef WINDOW_PROJECT_H
#define WINDOW_PROJECT_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <project/project.h>
#include <serial/serialthread.h>
#include <parsers/dataparserfactory.h>
#include <ui/chartwidget.h>
#include <ui/variablestablemodel.h>

namespace Ui {
class WindowProject;
}

class WindowProject : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowProject)
public:
    WindowProject(QSharedPointer<Project> project, QWidget *parent = 0);
    ~WindowProject();

    QSharedPointer<Project> project() const;

    void exportImages(QString const& filePath);
    void setNightView(bool enabled);

private:
    void createChart(Chart const& chart, QList<Channel> const& channels);
    void createCharts(QList<Chart> const& charts, QList<Channel> const& channels);
    void clearCharts();

private slots:
    void onChartMinimumHeightChanged(int);
    void onChartEditRequested();
    void on_actionConnect_triggered();
    void on_actionPause_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSerial_Configuration_triggered();
    void on_actionChannels_triggered();
    void on_actionCharts_triggered();

signals:
    void startProcessing();

private:
    Ui::WindowProject *ui;
    QSharedPointer<Project> m_project;
    QScopedPointer<SerialThread> m_serial;
    DataParserFactory m_parserFactory;

    QList<ChartWidget*> m_charts;
};

#endif // WINDOW_PROJECT_H
