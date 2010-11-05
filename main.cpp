#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(100,100,1024,300);
    w.show();

    return a.exec();
}
