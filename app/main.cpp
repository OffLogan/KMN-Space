#include <QApplication>

#include "../include/homescreen.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    homeScreen window;
    window.show();
    return app.exec();
}
