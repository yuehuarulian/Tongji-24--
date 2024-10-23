#include "stationqgraphicsview.h"

#include <cmath>
#include <QDebug>
#include <QWheelEvent>

StationQGraphicsView::StationQGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    reset_zoom();
}

void StationQGraphicsView::refresh()
{
    this->viewport()->update();
}

void StationQGraphicsView::zoom_in()
{
    zoom(1.2);
}

void StationQGraphicsView::zoom_out()
{
    zoom(0.833);
}

void StationQGraphicsView::zoom(double scaleFactor)
{
    //缩放函数
    qreal factor = transform().scale(scaleFactor,scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 50)
        return;
    scale(scaleFactor, scaleFactor);
}

void StationQGraphicsView::reset_zoom()
{
    qreal init_scale = 1;
    this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    QTransform transform;
    transform.scale(init_scale, init_scale);

    this->setTransform(transform, false);
}
