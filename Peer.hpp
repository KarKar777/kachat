#pragma once
#include <atomic>
#include <chrono>
#include <memory>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

#include "Connection.hpp"
#include "Utils.hpp"

class Peer {
   public:
    Peer(int port);
    ~Peer();
    void run();    // running peer (blocking)
    void start();  // startint peer
    void stop();
    void connectTo(const std::string& ip, int port);
    void broadcast(const std::string& msg);  // send to all connections
    void cli();

   private:
    void acceptLoop();  // accepting new connections

    int listenSock = -1;
    int port;

    std::vector<Connection*> connections;
    std::vector<std::thread> threads;
    std::atomic<bool> running{false};
    std::unordered_set<size_t> messageHashes;
};
