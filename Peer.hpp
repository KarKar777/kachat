#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "Connection.hpp"
class Peer {
   public:
    Peer(int port);
    ~Peer();
    void run(); // running peer (blocking)
    void start(); // startint peer
    void connectTo(const std::string& ip, int port); 
    void broadcast(const std::string& msg);  // send to all connections

   private:
    void acceptLoop();  // accepting new connections

    int listenSock = -1;
    int port;
    std::vector<std::shared_ptr<Connection>> connections;
    std::vector<std::thread> threads;
    std::atomic<bool> running{false};
};
