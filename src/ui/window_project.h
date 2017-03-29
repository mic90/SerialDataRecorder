#ifndef WINDOW_PROJECT_H
#define WINDOW_PROJECT_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <project/project.h>
#include <serial/serialthread.h>
#include <parsers/dataparserfactory.h>
#include <ui/chartwidget.h>

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

    bool hasUnsavedChanges() const;
    void setHasUnsavedChanges(bool hasUnsavedChanges);

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void createChart(Chart const& chart, QList<Channel> const& channels);
    void createCharts(QList<Chart> const& charts, QList<Channel> const& channels);
    void clearCharts();
    void updateWindowTitle();

private slots:
    void onProjectChanged();
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
    void savePossibleChanged(bool);

private:
    Ui::WindowProject *ui;
    QSharedPointer<Project> m_project;
    QScopedPointer<SerialThread> m_serial;
    DataParserFactory m_parserFactory;
    QList<ChartWidget*> m_charts;

    bool m_hasUnsavedChanges;
};

#endif // WINDOW_PROJECT_H
