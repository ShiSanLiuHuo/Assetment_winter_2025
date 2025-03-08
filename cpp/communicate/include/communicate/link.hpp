#ifndef LINK_HPP
#define LINK_HPP

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

class Link {
protected:
    bool debug_;
    std::chrono::time_point<std::chrono::system_clock> last_incoming_time;
    std::chrono::time_point<std::chrono::system_clock> last_outgoing_time;

    // 记录传入数据的时间戳
    void record_incoming_time() {
        last_incoming_time = std::chrono::system_clock::now();
        if (debug_) {
            std::cout << "Incoming data at: " << std::chrono::system_clock::to_time_t(last_incoming_time) << std::endl;
        }
    }

    // 记录传出数据的时间戳
    void record_outgoing_time() {
        last_outgoing_time = std::chrono::system_clock::now();
        if (debug_) {
            std::cout << "Outgoing data at: " << std::chrono::system_clock::to_time_t(last_outgoing_time) << std::endl;
        }
    }

    // 异常处理函数
    void handle_exception(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

public:
    Link(bool debug = false) : debug_(debug) {}
    virtual ~Link() = default;
};

#endif