#include "server.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>

server::server(QWidget *parent)
    : QMainWindow(parent)
{
    // --- UI ---
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QHBoxLayout *topLayout = new QHBoxLayout();

    portEdit = new QLineEdit();
    portEdit->setPlaceholderText("Enter port");
    portEdit->setValidator(new QIntValidator(1, 65535));
    portEdit->setText("5000");

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

    setWindowTitle("Chat Server");
    resize(500, 400);

    // TCP
    tcpServer = new QTcpServer(this);

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

    if (!tcpServer->listen(QHostAddress::Any, port)) {
        log("❌ Failed to start server.");
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &server::newConnection);

    log("✔ Server started on port " + QString::number(port));

    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
}

void server::stopServer()
{
    foreach (QTcpSocket *c, clients) {
        c->disconnectFromHost();
    }
    clients.clear();

    tcpServer->close();

    log("⚠ Server stopped");

    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
}

void server::newConnection()
{
    QTcpSocket *client = tcpServer->nextPendingConnection();
    clients.append(client);

    log("🔵 Client connected");

    connect(client, &QTcpSocket::readyRead, this, &server::readClient);
}

void server::readClient()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    QString msg = client->readAll();

    log("📩 Message: " + msg);

    // broadcast
    for (QTcpSocket *c : clients)
        c->write(msg.toUtf8());
}
