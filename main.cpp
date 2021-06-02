#include "regulmainwindow.h"
#include "regulviewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RegulMainWindow w;
    w.show();
    return a.exec();
}
