#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMap>

#include "station.h"
#include "subwaysystem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 画地铁线路图
    void init_combobox();
    void draw_subway_system();
    void draw_transline(const QList<QString>&);

    // 添加菜单栏的三个action 链接槽函数
    void add_station();
    void add_line();
    void add_edge();

    // 查询线路
    void query_line();

private:
    Ui::MainWindow *ui;
    SubwaySystem subsys;
    QGraphicsScene *scene;   // 绘图场景

    void draw_station(const Station&,const QColor);
    void draw_edge(const Station&,const Station&,const QPen);
};
#endif // MAINWINDOW_H
