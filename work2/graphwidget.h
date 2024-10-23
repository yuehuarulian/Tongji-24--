#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <vector>

class GraphWidget : public QWidget
{
public:
    GraphWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setNodesAndEdges(const std::vector<QPoint> &nodes, const std::vector<std::pair<int, int>> &edges)
    {
        this->nodes = nodes;
        this->edges = edges;
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        // 设置画笔用于绘制连线
        QPen pen(Qt::black, 2);
        painter.setPen(pen);

        // 绘制边 (连线)
        for (const auto &edge : edges)
        {
            const QPoint &startNode = nodes[edge.first];
            const QPoint &endNode = nodes[edge.second];
            painter.drawLine(startNode, endNode);
        }

        // 设置画笔用于绘制节点
        QBrush brush(Qt::blue);
        painter.setBrush(brush);

        // 绘制节点
        for (const QPoint &node : nodes)
        {
            painter.drawEllipse(node, 5, 5);  // 绘制半径为5的圆形节点
        }
    }

private:
    std::vector<QPoint> nodes;                     // 存储节点位置
    std::vector<std::pair<int, int>> edges;        // 存储边，使用节点索引的对
};



#endif // GRAPHWIDGET_H
