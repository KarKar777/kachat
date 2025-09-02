#pragma once
#include <chrono>
#include <random>

#include "Connection.hpp"

std::string getRID();

struct Message {
    Message() {}
    Message(std::string msg) {
        TTL = 128;
        rid = getRID();
        message = msg;
    }

    int TTL;
    std::string rid;
    std::string message;
};



Message parseBuf(char* buf, int n);
size_t hashMessage(const Message& msg);
std::string makeMessage(const Message& msg);