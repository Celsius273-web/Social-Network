#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "network.h"
#include "user.h"
#include "post.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
