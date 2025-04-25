#include "server.h"
#include <QCoreApplication>  // 使用 QCoreApplication 代替 QApplication

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);  // 使用 QCoreApplication

    // 创建服务器对象
    Server server;

    // 进入事件循环
    return a.exec();
}
