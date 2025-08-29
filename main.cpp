// C++ program to illustrate the client application in the
// socket programming
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <Peer.hpp>
#include <cstring>
#include <iostream>
int main() {
    Peer peer(12345);
    peer.run();
    return 0;
}