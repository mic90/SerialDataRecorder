#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = 0);
    ~ChartWidget();

private:
    Ui::ChartWidget *ui;
};

#endif // CHARTWIDGET_H
