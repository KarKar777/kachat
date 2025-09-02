#include "Utils.hpp"

#include <chrono>
#include <random>

std::mt19937_64 rng(
    std::chrono::steady_clock::now().time_since_epoch().count());

std::string getRID() { return std::to_string(rng()); }

Message parseBuf(char* buf, int n) {
    Message msg;
    int i = 0;
    std::string TTL_str = "";
    std::string rid_str = "";
    std::string message_str = "";
    while (i < n && buf[i] != 30) {
        TTL_str += buf[i];
        i++;
    }
    i++;
    while (i < n && buf[i] != 30) {
        rid_str += buf[i];
        i++;
    }
    i++;
    while (i < n) {
        message_str += buf[i];
        i++;
    }
    msg.TTL = std::stoi(TTL_str) - 1;
    msg.rid = rid_str;
    msg.message = message_str;
    return msg;
}

size_t hashMessage(const Message& msg) {
    std::hash<std::string> hasher;
    return hasher(msg.rid + msg.message);
}

std::string makeMessage(const Message& msg) {
    return std::to_string(msg.TTL) + (char)30 + msg.rid + (char)30 + msg.message;
}