// ==================================
// File: client_main.cpp
// Shared Memory Chat CLIENT GUI
// ==================================

#include "client_window.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ClientWindow client;
    client.show();

    return app.exec();
}

