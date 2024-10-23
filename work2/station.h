#ifndef STATION_H
#define STATION_H

#include <QString>
#include <QSet>
#include <QPointF>
#include <QMap>

extern double min_longi;
extern double min_lati;
extern double max_longi;
extern double max_lati;

enum class State
{
    ERROR = -1,
    OK = 0,
    REPEAT = 1      //重复
};

struct Edge{
    QSet<QString> line_list;
    int dist = 0;
};

class Station
{
protected:
    QString name;
    double longi, lati;         // 经纬度
    QPointF coord;              // 图上的坐标位置
    QMap<QString, Edge> edges;   // 边：记录所有和name直接相连的station以及他们之间的距离，线路
    QSet<QString> lines;        // 线路

public:
    Station();

    void latilongi2coord(); // 经纬度转地图位置

    State add_edge(const Station&,const QString,const float = -1); // 添加一条从this到sta的位于line_name线上的边

    QString get_belonglines_text(); // 获取所有属于的线路线路

    friend int get_distance(const Station&,const Station&);
    friend class Line;
    friend class SubwaySystem;
    friend class MainWindow;
};

int get_distance(const Station&, const Station&); // 获取两个站点之间距离
void update_bound(const double, const double);

#endif // STATION_H
