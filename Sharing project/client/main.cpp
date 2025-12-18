#include <QApplication>
#include "client_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ClientWindow client;
    client.show();

    return app.exec();
}

