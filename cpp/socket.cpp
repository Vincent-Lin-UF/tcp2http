#include "socket.hpp"

#include <unistd.h>

Socket::Socket() noexcept
    : fd_(-1) {}

Socket::Socket(int fd)
    : fd_(fd) {
    if (fd_ < 0) {
        throw std::runtime_error("Invalid socket file descriptor");
    }
}

Socket::Socket(Socket&& other) noexcept
: fd_(other.fd_) {
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close_if_needed();
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

Socket::~Socket() {
    close_if_needed();
}

int Socket::fd() const noexcept {
    return fd_;
}

bool Socket::valid() const noexcept {
    return fd_ >= 0;
}

void Socket::close_if_needed() noexcept {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}