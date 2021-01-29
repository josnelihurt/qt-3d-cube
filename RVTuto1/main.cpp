// Cours de Réalité Virtuelle
// leo.donati@univ-cotedazur.fr
//
// EPU 2019-20
//
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Réalité Virtuelle: Tuto1");
    w.show();
    return a.exec();
}
