#ifndef SUBWAYCONTROLWINDOW_H
#define SUBWAYCONTROLWINDOW_H

#include <QWidget>
#include <QString>
#include <QColor>

#include "subwaysystem.h"

#define USE_CHECKBOX_LIST 0

namespace Ui {
class SubwayControlWindow;
}

class SubwayControlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SubwayControlWindow(int,SubwaySystem*);
    ~SubwayControlWindow();

    void init_tab_line();
    void init_tab_station();
    void init_tab_edge();
    void submit_tab_line();
    void submit_tab_station();
    void submit_tab_edge();

    void error_notice(QString);
    void right_notice(QString);

private:
    Ui::SubwayControlWindow *ui;

    QString name;
    QColor color;
    SubwaySystem*subsys;
    double longi,lati;
    void get_color();

signals:
    void done();
};

#endif // SUBWAYCONTROLWINDOW_H
