#include "server.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QMetaObject>

server::server(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    QHBoxLayout *topLayout = new QHBoxLayout();

    portEdit = new QLineEdit();
    portEdit->setPlaceholderText("Enter port");
    portEdit->setValidator(new QIntValidator(1, 65535));
    portEdit->setText("9000");

    startBtn = new QPushButton("Start Server");
    stopBtn  = new QPushButton("Stop");
    stopBtn->setEnabled(false);

    topLayout->addWidget(portEdit);
    topLayout->addWidget(startBtn);
    topLayout->addWidget(stopBtn);

    logBox = new QTextEdit();
    logBox->setReadOnly(true);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(logBox);

    setWindowTitle("Chat Server GUI");
    resize(600, 400);

    core = nullptr;

    connect(startBtn, &QPushButton::clicked, this, &server::startServer);
    connect(stopBtn,  &QPushButton::clicked, this, &server::stopServer);
}

server::~server() {}

void server::log(QString msg)
{
    logBox->append(msg);
}

void server::startServer()
{
    int port = portEdit->text().toInt();

    core = new ServerCore();

    core->logCallback = [&](const std::string& m) {
        QMetaObject::invokeMethod(this, [=] {
            log(QString::fromStdString(m));
        });
    };

    if (!core->start(port)) {
        log("❌ Failed to start server");
        delete core;
        core = nullptr;
        return;
    }

    log("✔ ServerCore started");

    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
}

void server::stopServer()
{
    if (core) {
        core->stop();
        delete core;
        core = nullptr;
    }

    log("⚠ Server stopped by GUI");

    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
}

