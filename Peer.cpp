#include "Peer.hpp"

Peer::Peer(int port) : port(port) {}

Peer::~Peer() {
    if (!running) return;

    running = false;

    if (listenSock >= 0) {
        shutdown(listenSock, SHUT_RDWR);
        close(listenSock);
        listenSock = -1;
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

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0) {
        perror("bind failed");
    }

    if (listen(listenSock, 5) < 0) {
        perror("listen failed");
    }
    sockaddr_in clientAddr{};
    socklen_t len = sizeof(clientAddr);

    threads.emplace_back([this]() { acceptLoop(); });
}

void Peer::run() {
    start();

    // blocking loop
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
            if (running) perror("accept failed");
            continue;
        }

        auto conn = std::make_shared<Connection>(clientSock);
        connections.push_back(conn);

        threads.emplace_back([conn]() { conn->recvloop(); });
    }
}
