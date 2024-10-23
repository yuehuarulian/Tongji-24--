#include "station.h"
#include <cmath>
#include <QDebug>

double min_longi;
double min_lati;
double max_longi;
double max_lati;

Station::Station() {}

void Station::latilongi2coord()
{
    coord.setX((longi - min_longi) / (max_longi - min_longi) * 3000 + 30); ///
    // 使用了 maxLati - this->lati 来反转 Y 轴，因为屏幕坐标系通常 Y 轴向下增加
    coord.setY((max_lati - lati) / (max_lati - min_lati) * 3000 + 30); ///
}

State Station::add_edge(const Station& sta, const QString line_name, const float distance)
{
    if(edges.count(sta.name)) {  // 如果已经有线路连接了
        int len = edges[sta.name].line_list.size();
        edges[sta.name].line_list.insert(line_name); // 加入当前线路
        return (len == edges[sta.name].line_list.size())? State::ERROR: State::OK; // 如果已经有线路了表示重复添加了
    }

    Edge edge;
    edge.line_list.insert(line_name);
    if(distance > 0)
        edge.dist = distance;
    else if(sta.edges.count(name))
        edge.dist = sta.edges[name].dist;
    else
        edge.dist = get_distance(*this, sta);

    edges[sta.name] = edge;

    // qDebug()<<edge.dist;

    return State::OK;
}

QString Station::get_belonglines_text()
{
    QString ret;
    for(auto &i:this->lines)
        ret+=i+' ';
    return ret;
}

static double HaverSin(double theta)
{
    double v = sin(theta / 2);
    return v * v;
}

int get_distance(const Station &sta1, const Station &sta2)
{
    const double EARTH_RADIUS = 6378.137;

    double rlati1 = sta1.lati * M_PI / 180;
    double rlati2 = sta2.lati * M_PI / 180;
    double rlongi1 = sta1.longi * M_PI / 180;
    double rlongi2 = sta2.longi * M_PI / 180;

    double vlongi = abs(rlongi1-rlongi2);
    double vlati = abs(rlati1-rlati2);

    double h = HaverSin(vlati)+cos(rlati1)*cos(rlati2)*HaverSin(vlongi);
    double ret = 2 * EARTH_RADIUS * asin(sqrt(h));

    return (int)(ret*1000);
}

void update_bound(const double lati,const double longi)
{
    if(min_lati>lati)
        min_lati=lati;
    else if(max_lati<lati)
        max_lati=lati;
    if(min_longi>longi)
        min_longi=longi;
    else if(max_longi<longi)
        max_longi=longi;
}
