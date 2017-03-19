#include "resizablescrollarea.h"
#include <QWheelEvent>
#include <QGuiApplication>
#include <QDebug>

ResizableScrollArea::ResizableScrollArea(QWidget *parent) : QScrollArea(parent)
{

}

void ResizableScrollArea::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers() & Qt::ControlModifier)
    {
        return;
    }
    QScrollArea::wheelEvent(event);
}
