#ifndef RESIZABLESCROLLAREA_H
#define RESIZABLESCROLLAREA_H

#include <QScrollArea>

class ResizableScrollArea : public QScrollArea
{
    Q_OBJECT
    Q_DISABLE_COPY(ResizableScrollArea)
public:
    ResizableScrollArea(QWidget *parent = 0);
    ~ResizableScrollArea() = default;

protected:
    virtual void wheelEvent(QWheelEvent *event) override;
};

#endif // RESIZABLESCROLLAREA_H
