#include <iostream>
#include <string>
#include <system_error>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/inet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

#include "socket.hpp"

static void throw_sys_error(const std::string& msg) {
    thorw std::system_error(errno, std::generic_category(), msg);
}

Socket create_listening_socket(uint16_t port) {
    // socket(int domain, int type, int protocol)
    // 0 protocl is an unspecified default protocol app for the rqe socket type 
    // https://pubs.opengroup.org/onlinepubs/009696599/functions/socket.html
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw_sys_error("socket");
    }

    Socket sock(fd);

    // allow it to have a quick restart on the port
    // setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
    // https://pubs.opengroup.org/onlinepubs/009695099/functions/setsockopt.html
    int opt = 1;
    if (::setsockopt(sock.fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw_sys_error("bind");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    // bind(int socket, const struct sockaddr *address, socklen_t address_len)
    // https://pubs.opengroup.org/onlinepubs/009695399/functions/bind.html
    if(::bind(sock.fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        throw_sys_error("bind");
    }

    if (::listen(sock.fd(), SOMAXCONN) < 0) {
        throw_sys_error("listen");
    }

    return sock;
}