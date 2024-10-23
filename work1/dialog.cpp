#include "dialog.h"
#include "ui_dialog.h"
#include "linkedlist.h"
#include <QMouseEvent>
#include <QDebug>
const int SINGLE = 0;
const int CIRCULAR = 1;
const int DOUBLE = 2;

SinglyLinkedList linkedlist;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_ok_clicked()
{
    ui->textBrowser->clear();
    int type_choose = ui->radioButton_single->isChecked() * SINGLE +
                      ui->radioButton_circular->isChecked() * CIRCULAR +
                      ui->radioButton_double->isChecked() * DOUBLE;

    int num = ui->spinBox_num->value();
    QString str = "";
    if (ui->radioButton_create->isChecked())
    {
        linkedlist.clear();
        str += "\n已重新建立列表\n";
        qDebug() << "radioButton_create";
    }
    else if (ui->radioButton_insert->isChecked())
    {
        linkedlist.insert(num);
        str += "\n已插入" + std::to_string(num) + '\n';
        qDebug() << "radioButton_insert";
    }
    else if (ui->radioButton_delete->isChecked())
    {
        if (linkedlist.remove(num))
            str += "\n已删除" + std::to_string(num) + '\n';
        else
            str += "\n没有找到待删除值\n";
        qDebug() << "radioButton_delete";
    }
    else if (ui->radioButton_find->isChecked())
    {
        int count = 0;
        auto node = linkedlist.find(num, count);
        if (node == nullptr)
            str += "\n未找到" + std::to_string(num) + '\n';
        else
        {
            str += "\n找到" + std::to_string(num) + "，是第" + std::to_string(count) + "个值" + '\n';
            str += "如果有多个相同查找值以第一个为准\n";
        }
        qDebug() << "radioButton_find";
    }

    if (type_choose == SINGLE)
    {
        str += QString::fromStdString(linkedlist.single_display());
    }
    else if (type_choose == CIRCULAR)
    {
        str += QString::fromStdString(linkedlist.circular_display());
    }
    else if (type_choose == DOUBLE)
    {
        str += QString::fromStdString(linkedlist.double_display());
    }
    ui->textBrowser->setText(str);
    qDebug() << str;
}

void Dialog::on_radioButton_single_clicked()
{
    QString str = "";
    ui->textBrowser->clear();
    linkedlist.clear();
    str += "已重新建立单向列表\n";
    ui->textBrowser->setText(str);
    qDebug() << str;
}

void Dialog::on_radioButton_circular_clicked()
{
    QString str = "";
    ui->textBrowser->clear();
    linkedlist.clear();
    str += "已重新建立循环列表\n";
    ui->textBrowser->setText(str);
    qDebug() << str;
}

void Dialog::on_radioButton_double_clicked()
{
    QString str = "";
    ui->textBrowser->clear();
    linkedlist.clear();
    str += "已重新建立双向列表\n";
    ui->textBrowser->setText(str);
    qDebug() << str;
}

// 三个鼠标事件的重写
// 鼠标释放
void Dialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) // 鼠标左键释放
        last = e->globalPosition();
}

// 鼠标移动
void Dialog::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) // 坐标左键按下并移动
    {
        int dx = e->globalPosition().x() - last.x();
        int dy = e->globalPosition().y() - last.y();
        last = e->globalPosition();
        move(x() + dx, y() + dy);
    }
}

// 鼠标按下
void Dialog::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) // 鼠标左键按下
    {
        int dx = e->globalPosition().x() - last.x();
        int dy = e->globalPosition().y() - last.y();
        move(x() + dx, y() + dy);
    }
}
