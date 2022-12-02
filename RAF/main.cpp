#include "main_window.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    system("chcp 1251"); // кирилица
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
