#include <QDebug>
#include <QString>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <random>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stationqgraphicsview.h"
#include "subwaycontrolwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("上海的地铁交通网路");
    srand(0);

    // 菜单栏    // 工具栏
    connect(ui->action_addstation, &QAction::triggered, this, &MainWindow::add_station);
    connect(ui->action_addedge, &QAction::triggered, this, &MainWindow::add_edge);
    connect(ui->action_addline, &QAction::triggered, this, &MainWindow::add_line);
    connect(ui->action_subway, &QAction::triggered, this, &MainWindow::draw_subway_system);
    connect(ui->action_zoomin, &QAction::triggered, ui->graphicsView,&StationQGraphicsView::zoom_in);
    connect(ui->action_zoomout, &QAction::triggered, ui->graphicsView,&StationQGraphicsView::zoom_out);

    // 连接按钮
    connect(ui->pushButton_search,&QPushButton::clicked,this,&MainWindow::query_line);

    // 调整场景
    this->scene = new QGraphicsScene;
    // scene->setBackgroundBrush(Qt::white);
    scene->setSceneRect(0,0,3080,3080);

    // 画板
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);     // 设置精致绘图
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    this->subsys.readSubwayFile(":/source/subway_info.txt");     // 读入地铁信息
    for(auto &i:subsys.stations)                                 // 转换坐标
        i.latilongi2coord();
    init_combobox();                                             // 设置combobox
    draw_subway_system();                                        // 画图
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_combobox()
{
    // 下拉显示
    QStringList sta_name_list;
    for(auto &i:this->subsys.stations.keys())
        sta_name_list.append(i);
    std::sort(sta_name_list.begin(),sta_name_list.end(),[](const QString &s1, const QString &s2){
        return (s1.localeAwareCompare(s2) < 0);
    });
    ui->comboBox->addItems(sta_name_list);
    ui->comboBox_2->addItems(sta_name_list);

    // 设置提示词
    QLineEdit *line1 = new QLineEdit;
    line1->setPlaceholderText("起点");
    ui->comboBox->setLineEdit(line1);
    ui->comboBox->lineEdit()->clear();

    QLineEdit *line2 = new QLineEdit;
    line2->setPlaceholderText("终点");
    ui->comboBox_2->setLineEdit(line2);
    ui->comboBox_2->lineEdit()->clear();
}

void MainWindow::draw_subway_system()
{
    this->scene->clear();
    ui->graphicsView->refresh();

    bool repeat;
    for(auto &line:this->subsys.lines){
        for(int i=0;i<line.sta_list.size();i++){
            for(int j=0;j<line.sta_list.at(i).size()-1;j++){
                QString s1=line.sta_list.at(i).at(j),s2=line.sta_list.at(i).at(j+1);
                repeat = false;
                //检查是否重复
                for(int m=0;m<i;m++){
                    for(int n=0;n<line.sta_list.at(m).size()-1;n++){
                        if(line.sta_list.at(m).at(n)==s1&&line.sta_list.at(m).at(n+1)==s2){
                            repeat = true;
                            break;
                        }
                    }
                    if(repeat)
                        break;
                }
                if(repeat)
                    continue;
                else
                    draw_edge(this->subsys.stations[s1],this->subsys.stations[s2],line.color);
            }
        }
    }

    for(auto &i:subsys.stations){
        draw_station(i,(i.lines.size()==1)?this->subsys.lines[*i.lines.begin()].color:Qt::black);
    }
}

// 查询完成后显示线路文本和路径
void MainWindow::draw_transline(const QList<QString> & trans_line)
{
    //重置
    draw_subway_system();
    ui->textBrowser->clear();

    // 设置格式
    QTextCursor cursor = ui->textBrowser->textCursor();
    QTextCharFormat format1, format2;
    format1.setFontWeight(QFont::Normal);
    format1.setFontPointSize(10);
    format2.setFontWeight(QFont::Bold);
    format2.setFontPointSize(10);

    QString line_old = *this->subsys.stations[trans_line.at(0)].edges[trans_line.at(1)].line_list.begin();

    QPen pen(QColor(125,205,242));
    pen.setWidth(5);  // 设置线条宽度，例如 2 像素
    draw_edge(this->subsys.stations[trans_line.at(0)],this->subsys.stations[trans_line.at(1)], pen);
    draw_station(this->subsys.stations[trans_line.at(0)],QColor(125,205,242));

    cursor.insertText("共经过"+QString::number(trans_line.size())+"站"+'\n', format2);
    cursor.insertText("起点: "+trans_line.at(0)+'\n');
    cursor.insertText("乘坐" + this->subsys.lines[line_old].name + "\n");
    cursor.insertText("        ↓\n", format1);


    for(int i=1;i<trans_line.size()-1;i++){
        // ui->textBrowser->append("\t"+trans_line.at(i));
        cursor.insertText(trans_line.at(i)+'\n', format1);
        bool line_name_same=false;
        for(auto &i:this->subsys.stations[trans_line.at(i)].edges[trans_line.at(i+1)].line_list)
            if(i==line_old){
                line_name_same=true;
                break;
            }
        if(line_name_same) {
            //线是一致的,则判断是否为站内换乘即可
            bool line_same=false;
            for(auto &sta_list:this->subsys.lines[line_old].sta_list){
                for(int k=1;k<sta_list.size()-1;k++){
                    if((trans_line.at(i-1)==sta_list.at(k-1)&&trans_line.at(i)==sta_list.at(k)&&trans_line.at(i+1)==sta_list.at(k+1))||
                        (trans_line.at(i+1)==sta_list.at(k-1)&&trans_line.at(i)==sta_list.at(k)&&trans_line.at(i-1)==sta_list.at(k+1))){
                        line_same = true;
                        break;
                    }
                }
                if(line_same)
                    break;
            }
            if(line_same)
                cursor.insertText("        ↓\n", format1);
            else
                cursor.insertText("        ↓站内换乘\n", format1);
        }
        else {
            line_old = *this->subsys.stations[trans_line.at(i)].edges[trans_line.at(i+1)].line_list.begin();
            cursor.insertText("        ↓\n", format1);
            cursor.insertText("换乘"+this->subsys.lines[line_old].name+'\n', format2);
            cursor.insertText("        ↓\n", format1);
        }

        QPen pen(QColor(125,205,242));
        pen.setWidth(5);  // 设置线条宽度，例如 2 像素
        draw_edge(this->subsys.stations[trans_line.at(i)],this->subsys.stations[trans_line.at(i+1)], pen);
        draw_station(this->subsys.stations[trans_line.at(i)], QColor(125,205,242));
    }
    draw_station(this->subsys.stations[trans_line.last()], QColor(125,205,242));
    cursor.insertText("终点: "+trans_line.last(), format2);
}

void MainWindow::add_line()
{
    SubwayControlWindow *sub_window = new SubwayControlWindow(1,&this->subsys);
    sub_window->show();
}

void MainWindow::add_station()
{
    SubwayControlWindow *sub_window = new SubwayControlWindow(2,&this->subsys);
    sub_window->show();
    connect(sub_window,&SubwayControlWindow::done,this,&MainWindow::draw_subway_system);
}

void MainWindow::add_edge()
{
    SubwayControlWindow *sub_window = new SubwayControlWindow(0,&this->subsys);
    sub_window->show(); // 显示ui
    connect(sub_window,&SubwayControlWindow::done,this,&MainWindow::draw_subway_system); // 如果出发了done信号就会执行draw_subway_system函数
}

void MainWindow::query_line()
{
    QString start_sta = ui->comboBox->lineEdit()->text();
    QString end_sta = ui->comboBox_2->lineEdit()->text();
    qDebug()<<start_sta<<" "<<end_sta;

    if(start_sta=="") {
        QMessageBox::critical(this,"错误","尚未输入起点站");
        return;
    } else if(end_sta==""){
        QMessageBox::critical(this,"错误","尚未输入终点站");
        return;
    } else if(this->subsys.stations.count(start_sta)==0) {
        QMessageBox::critical(this,"错误","站点\""+start_sta+"\"不存在\n请重新输入");
        return;
    } else if(this->subsys.stations.count(end_sta)==0) {
        QMessageBox::critical(this,"错误","站点\""+end_sta+"\"不存在\n请重新输入");
        return;
    } else if(start_sta==end_sta){
        QMessageBox::critical(this,"错误","起点站和终点站相同\n请重新输入");
        return;
    }

    QList<QString> ret_line;
    ret_line = this->subsys.shortTimePath(start_sta, end_sta);
    qDebug()<<ret_line;
    draw_transline(ret_line);
}

// 画一个站点
void MainWindow::draw_station(const Station& sta,const QColor color)
{
    // 初始化画笔
    QPen elli_pen;
    elli_pen.setColor(color);

    // 绘制椭圆形
    QGraphicsEllipseItem * elli = new QGraphicsEllipseItem(); // 用于绘制椭圆形
    elli->setRect(QRect(sta.coord.x(),sta.coord.y(),6,6)); // 设置椭圆形的矩形边界，通过 sta.coord 获取站点的坐标，并且设置宽度和高度为 6 像素
    elli->setPen(elli_pen);
    elli->setBrush(QBrush(Qt::white));

    // 字符串 info，用于存储站点的信息，包括站点名称、经纬度等
    QString info = "站点名称: "+sta.name+"\n站点经纬度: "+
                   QString::number(sta.longi,'f',3)+
                   ","+QString::number(sta.lati,'f',3)+
                   "\n站点所属线路: ";
    if(sta.lines.size())
        for(auto &i:sta.lines)
            info+=i+' ';
    else
        info+="无";
    info+="\n相邻站点:";
    if(sta.edges.size())
        for(auto &i:sta.edges.keys()){
            info+="\n "+i;
            if(i.length()<=3)
                info+="      ";
            info+="\t相距"+QString::number(sta.edges[i].dist/1000.0)+"km";
        }
    else
        info+="无";
    //    elli->setToolTip(info);

    this->scene->addItem(elli);

    QGraphicsTextItem * text = new QGraphicsTextItem();
    text->setPlainText(sta.name);
    text->setPos(sta.coord.x() + 5, sta.coord.y() - 8);
    text->setFont(QFont("helvetica",10,1));
    text->setToolTip(info);
    text->setDefaultTextColor(Qt::black);
    this->scene->addItem(text);
}

// 画一条边
void MainWindow::draw_edge(const Station&s1,const Station&s2,const QPen pen)
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    QPointF pos1,pos2;

    if(s1.lines.size()>1){
        pos1.setX(s1.coord.x()+(1.0*rand()/RAND_MAX-0.5)*4+3);
        pos1.setY(s1.coord.y()+(1.0*rand()/RAND_MAX-0.5)*4+3);
    }
    else{
        pos1.setX(s1.coord.x()+3);
        pos1.setY(s1.coord.y()+3);
    }

    if(s2.lines.size()>1){
        pos2.setX(s2.coord.x()+(1.0*rand()/RAND_MAX-0.5)*4+3);
        pos2.setY(s2.coord.y()+(1.0*rand()/RAND_MAX-0.5)*4+3);
    }
    else{
        pos2.setX(s2.coord.x()+3);
        pos2.setY(s2.coord.y()+3);
    }
    line->setLine(QLineF(pos1,pos2));

    line->setPen(pen);

    this->scene->addItem(line);
}
