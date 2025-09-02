#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <unordered_set>

#include "Utils.hpp"
class Connection {
   public:
    Connection(int sock);
    ~Connection();
    void sendMsg(const std::string& msg);
    void recvloop(std::unordered_set<size_t>& messageHashes);

   private:
    const size_t BUF_SIZE = 1024;
    int sock;
};