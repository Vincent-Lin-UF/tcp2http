#pragma once

#include <stdexcept>

class Socket {
public:
    Socket() noexcept;

    explicit Socket(int fd);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    ~Socket();

    int fd() const noexcept;
    bool valid() const noexcept;

private:
    int fd_;

    void close_if_needed() noexcept;

};