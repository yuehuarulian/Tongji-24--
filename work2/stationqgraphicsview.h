#ifndef STATIONQGRAPHICSVIEW_H
#define STATIONQGRAPHICSVIEW_H

#include <QGraphicsView>

class StationQGraphicsView : public QGraphicsView
{
public:
    StationQGraphicsView(QWidget *parent = nullptr);

    void zoom_in();
    void zoom_out();
    void reset_zoom();
    void refresh();

private:
    void zoom(double);

};

#endif // STATIONQGRAPHICSVIEW_H
