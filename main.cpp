#include <iostream>

#include "Peer.hpp"
int main() {
    std::cout << "Hi! This is a kachat - p2p messenger." << std::endl;
    std::cout << "Enter port: " << std::flush;
    int port;
    std::cin >> port;

    Peer peer(port);

    peer.run();
    return 0;
}