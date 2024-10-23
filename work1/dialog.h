#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_ok_clicked();

    void on_radioButton_single_clicked();

    void on_radioButton_circular_clicked();

    void on_radioButton_double_clicked();
private slots:
    //由于去掉显示界面的菜单栏和标题栏，无法拖动窗口，重写关于窗口拖动用的几个函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::Dialog *ui;

    QPointF last;//窗口拖动用变量
};
#endif // DIALOG_H
