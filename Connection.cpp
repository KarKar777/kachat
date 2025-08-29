#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <Connection.hpp>
#include <cstring>
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
    // std::cout << "OK\n";
    while (true) {
        int n = recv(sock, buf, BUF_SIZE - 1, 0);
        // if (n <= 0) break;
        for (int i = 0; i < n; i++) {
            std::cout << buf[i];
        }
        // std::cout << std::endl;
    }
}
