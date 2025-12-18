#include <QApplication>
#include "server_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ServerWindow server;
    server.show();

    return app.exec();
}

