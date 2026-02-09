#include <QApplication>
#include "include/gui/Mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // 1. Start Engine

    MainWindow w;                 // 2. Build Car (triggers Canvas creation)
    w.show();                     // 3. Reveal Car

    return app.exec();            // 4. Enter Loop (Wait for clicks)
}