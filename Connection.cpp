#include "Connection.hpp"

#include <iostream>
Connection::Connection(int sock) : sock(sock) {}
Connection::~Connection() {
    if (sock >= 0) {
        close(sock);
    }
}
void Connection::sendMsg(const std::string& msg) {
    std::string data = makeMessage(Message(msg));
    send(sock, data.c_str(), data.size(), 0);
}

void Connection::recvloop(std::unordered_set<size_t>& messageHashes) {
    char buf[BUF_SIZE];
    while (true) {
        int n = recv(sock, buf, BUF_SIZE - 1, 0);
        Message msg = parseBuf(buf, n);
        if (msg.TTL <= 0) {
            continue;
        }
        size_t hash = hashMessage(msg);
        if (messageHashes.find(hash) != messageHashes.end()) {
            continue;
            messageHashes.insert(hash);
        }
        std::cout << msg.message << std::endl;
    }
}
