#include <QApplication>
#include "fractale.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Fractale m(false);
    m.show();
    return app.exec();
}

