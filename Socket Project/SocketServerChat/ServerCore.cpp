#include "ServerCore.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>

using namespace std;
ServerCore::ServerCore() {}

ServerCore::~ServerCore() {
    stop();
}


bool ServerCore::start(int port) {
    if (serverRunning) return false;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        if (logCallback) logCallback("❌ Failed to bind server");
        return false;
    }

    if (listen(serverSock, 10) < 0) {
        if (logCallback) logCallback("❌ Failed to listen");
        return false;
    }

    serverRunning = true;

    if (logCallback) logCallback("✔ Server running on port " + to_string(port));

    acceptThread = thread(&ServerCore::acceptLoop, this);
    acceptThread.detach();

    return true;
}

void ServerCore::stop() {
    if (!serverRunning) return;
    serverRunning = false;

    shutdown(serverSock, SHUT_RDWR);
    close(serverSock);

    // Close all clients
    lock_guard<mutex> lock(mtx);
    for (int sock : clients) {
        shutdown(sock, SHUT_RDWR);
        close(sock);
    }
    clients.clear();

    if (logCallback) logCallback("⚠ Server stopped");
}

void ServerCore::acceptLoop() {
    while (serverRunning) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);

        int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &len);
        if (!serverRunning) return;

        if (clientSock >= 0) {
            {
                lock_guard<mutex> lock(mtx);
                clients.push_back(clientSock);
            }

            if (logCallback) logCallback("🔵 Client connected");

            thread(&ServerCore::clientHandler, this, clientSock).detach();
        }
    }
}

void ServerCore::clientHandler(int clientSock) {
    char buf[1024];

    // Receive username
    ssize_t n = recv(clientSock, buf, sizeof(buf) - 1, 0);
    if (n <= 0) {
        close(clientSock);
        return;
    }

    buf[n] = '\0';
    string firstMsg(buf);
    string username = "Unknown";

    if (firstMsg.rfind("/name ", 0) == 0)
        username = firstMsg.substr(6);

    {
        lock_guard<mutex> lock(mtx);
        clientNames[clientSock] = username;
    }

    if (logCallback) logCallback("[User joined] " + username);

    // Broadcast join
    {
        lock_guard<mutex> lock(mtx);
        for (int s : clients) {
            string j = "*** " + username + " joined the chat ***\n";
            send(s, j.c_str(), j.size(), 0);
        }
    }

    // Receive messages loop
    while (serverRunning) {
        n = recv(clientSock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) break;

        buf[n] = '\0';
        string msg(buf);

        if (logCallback) logCallback(username + ": " + msg);

        // Broadcast to others
        lock_guard<mutex> lock(mtx);
        for (int s : clients) {
            if (s != clientSock) {
                string fullMsg = username + ": " + msg;
                send(s, fullMsg.c_str(), fullMsg.size(), 0);
            }
        }
    }

    // Remove client
    {
        lock_guard<mutex> lock(mtx);
        clients.erase(remove(clients.begin(), clients.end(), clientSock), clients.end());
    }

    // Broadcast leave
    {
        string exitMsg = "*** " + username + " left the chat ***\n";
        for (int s : clients)
            send(s, exitMsg.c_str(), exitMsg.size(), 0);
    }

    close(clientSock);
}

