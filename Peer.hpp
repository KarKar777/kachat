#pragma once
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include "Connection.hpp"

class Peer {
   public:
    Peer(int port);
    ~Peer();
    void run();
    void start();
    void connectTo(const std::string& ip, int port);
    void broadcast(const std::string& msg);  // отправить всем

   private:
    void acceptLoop();  // слушает новые подключения

    int listenSock = -1;
    int port;
    std::vector<std::shared_ptr<Connection>> connections;
    std::vector<std::thread> threads;
    std::atomic<bool> running{false};
};
