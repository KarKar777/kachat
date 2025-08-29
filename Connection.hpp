#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

class Connection {
   public:
    Connection(int sock);
    ~Connection();
    void sendMsg(const std::string& msg);
    void recvloop();

   private:
    const size_t BUF_SIZE = 1024;
    int sock;
};