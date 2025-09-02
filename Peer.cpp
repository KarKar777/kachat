#include "Peer.hpp"

#include <chrono>
#include <iostream>
Peer::Peer(int port) : port(port) {}

Peer::~Peer() {
    running = false;

    if (listenSock >= 0) {
        shutdown(listenSock, SHUT_RDWR);
        close(listenSock);
        listenSock = -1;
    }

    connections.clear();
    for (auto &t : threads) {
        if (t.joinable() && std::this_thread::get_id() != t.get_id()) t.join();
    }
}
void Peer::stop() {
    running = false;

    if (listenSock >= 0) {
        shutdown(listenSock, SHUT_RDWR);
        close(listenSock);
        listenSock = -1;
    }

    connections.clear();
    for (auto &t : threads) {
        if (t.joinable() && std::this_thread::get_id() != t.get_id()) t.join();
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
    threads.emplace_back([this]() { cli(); });
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

        auto conn = new Connection(clientSock);
        connections.push_back(conn);

        threads.emplace_back([this, conn]() { conn->recvloop(messageHashes); });
    }
}

void Peer::broadcast(const std::string &msg) {
    for (auto conn : connections) {
        conn->sendMsg(msg);
    }
}

void Peer::connectTo(const std::string &ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == 0) {
        auto conn = new Connection(sock);
        connections.push_back(conn);
        threads.emplace_back([this, conn]() { conn->recvloop(messageHashes); });
        std::cout << "Connected to " << ip << ":" << port << std::endl;
    } else {
        std::cerr << "Connection failed" << std::endl;
        close(sock);
    }
}

void Peer::cli() {
    std::string opt;
    std::cout << ">>> " << std::flush;
    while (std::cin >> opt) {
        if (opt == "exit") {
            std::cout << "Bye!";
            stop();
            break;
        } else if (opt == "help") {
            std::cout << "There are such commands.\n";
            std::cout << "1. connect\n";
            std::cout << "2. broadcast\n";
            std::cout << "3. help\n";
            std::cout << "4. exit\n";
            std::cout << std::endl;
        } else if (opt == "connect") {
            std::cout << "Enter ip: " << std::flush;
            std::string ip;
            std::cin >> ip;
            std::cout << "Enter port: " << std::flush;
            int port;
            std::cin >> port;
            connectTo(ip, port);
        } else if (opt == "broadcast") {
            std::cout << "Enter a message" << std::endl;
            std::string msg;
            std::cin >> msg;
            broadcast(msg);
        } else {
            std::cout << "Invalid option. Try help" << std::endl;
        }
        std::cout << ">>> " << std::flush;
    }
}