#include "subwaycontrolwindow.h"
#include "ui_subwaycontrolwindow.h"
#include "station.h"

#include <QColorDialog>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QCheckBox>
#include <QGridLayout>

static void (SubwayControlWindow::*const initFun[])() = {
    &SubwayControlWindow::init_tab_edge,
    &SubwayControlWindow::init_tab_line,
    &SubwayControlWindow::init_tab_station
};

static const QString title[] = {
    "添加连线",
    "添加线路",
    "添加站点"
};

SubwayControlWindow::SubwayControlWindow(int index,SubwaySystem* subsys) :
    QWidget(nullptr),
    ui(new Ui::SubwayControlWindow)
{
    ui->setupUi(this);
    setWindowTitle(title[index]);
    setAttribute(Qt::WA_DeleteOnClose);
    this->subsys = subsys;

    ui->stackedWidget->setCurrentIndex(index);
    (this->*(initFun[index]))(); // 进如init函数
}

SubwayControlWindow::~SubwayControlWindow()
{
    delete ui;
}

void SubwayControlWindow::get_color()
{
    this->color = QColorDialog::getColor(Qt::white, this);
    ui->pushButton_color->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    qDebug()<<color;
}

void SubwayControlWindow::error_notice(QString prompt)
{
    QMessageBox::critical(this,"错误",prompt);
}

void SubwayControlWindow::right_notice(QString prompt)
{
    QMessageBox::information(this,"提示",prompt);
}

void SubwayControlWindow::init_tab_line()
{
    connect(ui->pushButton_color,&QPushButton::clicked,this,&SubwayControlWindow::get_color);
    connect(ui->pushButton_lineok,&QPushButton::clicked,this,&SubwayControlWindow::submit_tab_line);
    connect(ui->pushButton_lineclk, &QPushButton::clicked, this, &QWidget::close);
}

void SubwayControlWindow::init_tab_station()
{
    //设置spinbox范围
    ui->doubleSpinBox_longi->setRange(min_longi,max_longi);
    ui->doubleSpinBox_lati->setRange(min_lati,max_lati);
    connect(ui->pushButton_stationok,&QPushButton::clicked,this,&SubwayControlWindow::submit_tab_station);
    connect(ui->pushButton_stationclk, &QPushButton::clicked, this, &QWidget::close);
}

void SubwayControlWindow::init_tab_edge()
{
    //初始化combobox
    QStringList sta_name_list;
    QStringList line_name_list;
    for(auto &i:this->subsys->stations.keys())
        sta_name_list.push_back(i);
    for(auto &i:this->subsys->lines.keys())
        line_name_list.push_back(i);
    std::sort(sta_name_list.begin(),sta_name_list.end(),[](const QString &s1, const QString &s2){
        return (s1.localeAwareCompare(s2) < 0);
    });
    std::sort(line_name_list.begin(),line_name_list.end(),[](const QString &s1, const QString &s2){
        return (s1.length()!=s2.length())?(s1.length()<s2.length()):(s1<s2);
    });
    ui->comboBox_station1->addItems(sta_name_list);
    ui->comboBox_station2->addItems(sta_name_list);
    ui->comboBox_line->addItems(line_name_list);

    QLineEdit *line1 = new QLineEdit;
    line1->setPlaceholderText("请选择站点1");
    ui->comboBox_station1->setLineEdit(line1);
    ui->comboBox_station1->lineEdit()->clear();
    QLineEdit *line2 = new QLineEdit;
    line2->setPlaceholderText("请选择站点2");
    ui->comboBox_station2->setLineEdit(line2);
    ui->comboBox_station2->lineEdit()->clear();

    connect(ui->pushButton_edgeok, &QPushButton::clicked, this, &SubwayControlWindow::submit_tab_edge);
    connect(ui->pushButton_edgeclk, &QPushButton::clicked, this, &QWidget::close);
}

void SubwayControlWindow::submit_tab_line()
{
    this->name = ui->lineEdit->text();
    qDebug()<<this->name;
    qDebug()<<this->color;
    if(this->name==""){
        error_notice("未输入线路名称");
        return;
    }
    else if(this->subsys->lines.count(this->name)){
        error_notice("线路已存在,请重新输入");
        return;
    }
    else if(this->color==QColor::Invalid){
        error_notice("未选颜色！");
        return;
    }

    this->subsys->addLine(this->name,this->color);
    right_notice("线路"+this->name+"添加成功！");
    close();    //关闭窗口

    emit done();
}

void SubwayControlWindow::submit_tab_station()
{
    this->name = ui->lineEdit_stationname->text();
    if(this->name==""){
        error_notice("未输入站点名称");
        return;
    }
    // else if(this->name.length()>10){
    //     error_notice("站点名称不允许超过10个字符\n请重新输入");
    //     return;
    // }
    else if(this->subsys->stations.count(this->name)){
        error_notice("站点已存在,请重新输入");
        return;
    }

    this->longi = ui->doubleSpinBox_longi->value();
    this->lati = ui->doubleSpinBox_lati->value();

    QSet<QString> line_set;
    this->subsys->addStation(this->name,this->longi,this->lati,line_set);
    right_notice("站点"+this->name+"添加成功！");
    close();    //关闭窗口

    emit done();
}

void SubwayControlWindow::submit_tab_edge()
{
    QString text1,text2,text3;
    text1 = ui->comboBox_station1->lineEdit()->text();
    text2 = ui->comboBox_station2->lineEdit()->text();
    text3 = ui->comboBox_line->currentText();
    if(text1==""){
        error_notice("请输入站点1");
        return;
    }
    else if(text2==""){
        error_notice("请输入站点2");
        return;
    }
    else if(this->subsys->stations.count(text1)==0){
        error_notice("站点1不存在,请重新输入");
        return;
    }
    else if(this->subsys->stations.count(text2)==0){
        error_notice("站点2不存在,请重新输入");
        return;
    }
    else if(text1 == text2){
        error_notice("两个站点名不能相同");
        return;
    }
    // qDebug()<<text1<<text2<<text3;
    else if(this->subsys->addEdge(text1,text2,text3) == State::ERROR){
        error_notice("此路线已经存在，不可重复添加");
        return;
    }
    else
        right_notice("站点"+text1+"到站点"+text2+"的连接添加成功");
    close();    //关闭窗口

    emit done();
}
