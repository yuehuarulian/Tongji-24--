#include "dialog.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowFlags(Qt::FramelessWindowHint);// 设置没有窗口标题
    w.setAttribute(Qt::WA_TranslucentBackground, true);
    w.show();
    return a.exec();
}
