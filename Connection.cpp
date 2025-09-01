#include <Connection.hpp>
#include <iostream>
Connection::Connection(int sock) : sock(sock) {}
Connection::~Connection() {
    if (sock >= 0) {
        close(sock);
    }
}
void Connection::sendMsg(const std::string& msg) {
    std::string data = msg + "\n";
    send(sock, data.c_str(), data.size(), 0);
}

void Connection::recvloop() {
    char buf[BUF_SIZE];
    while (true) {
        int n = recv(sock, buf, BUF_SIZE - 1, 0);
        std::cout << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << buf[i];
        }
        std::cout << std::endl;
        std::cout << ">>> " << std::flush;
    }
}
