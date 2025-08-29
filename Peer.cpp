#include "Peer.hpp"

#include <mutex>
#include <string>
#include <thread>
#include <vector>

Peer::Peer(int port) : port(port) {}

Peer::~Peer() {
    if (!running) return;
    std::cout << "asd" << std::endl;
    running = false;
    if (listenSock >= 0) {
        shutdown(listenSock, SHUT_RDWR);
        close(listenSock);
    }
    connections.clear();
    for (auto &t : threads) {
        if (t.joinable()) t.join();
    }
}

void Peer::start() {
    if (running) return;
    running = true;

    listenSock = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    if (bind(listenSock, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0) {
        perror("bind failed");
    }

    // listening to the assigned socket
    if (listen(listenSock, 5) < 0) {
        perror("listen failed");
    }
    sockaddr_in clientAddr{};
    socklen_t len = sizeof(clientAddr);

    threads.emplace_back([this]() { acceptLoop(); });
}

void Peer::run() {
    start();

    // Блокирующий цикл — сервер живет пока running = true
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void Peer::acceptLoop() {
    while (running) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);

        int clientSock = accept(listenSock, (sockaddr *)&clientAddr, &len);
        if (clientSock < 0) {
            if (!running && (errno == EBADF || errno == ECONNABORTED)) break;
            if (errno == EINTR) continue;
            if (running) perror("accept failed");
            continue;
        }

        std::cout << "accepted" << std::endl;

        auto conn = std::make_shared<Connection>(clientSock);
        connections.push_back(conn);

        threads.emplace_back([conn]() { conn->recvloop(); });
    }
}
