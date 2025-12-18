#pragma once
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include <functional>

using namespace std;

class ServerCore {
public:
    ServerCore();
    ~ServerCore();

    bool start(int port);
    void stop();

    bool isRunning() const { return serverRunning; }

    // idon't know what is happen
    // Callback to send logs to GUI
    function<void(const string&)> logCallback;

private:
    void acceptLoop();
    void clientHandler(int clientSock);

private:
    int serverSock = -1;
    thread acceptThread;

    vector<int> clients;
    map<int, string> clientNames;
    mutex mtx;

    bool serverRunning = false;
};

