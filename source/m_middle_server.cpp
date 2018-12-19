
#include <m_middle_server.h>
#include <zconf.h>
#include <cstdio>
#include <netinet/in.h>
#include <cstring>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>

#define BUFFER_SIZE 1024

#define PORT 8080

#define ACCEPT_STATUS "ACCEPT"

#define ACCEPT_STATUS_SIZE 7

#define TAG "M_MIDDLE_SERVER"

using namespace lib4aio;

m_middle_server::m_middle_server()
{}

m_middle_server::~m_middle_server()
{

}

void m_middle_server::start()
{
    printf("m_middle_server has started!\n");
    while (true) {
        sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[BUFFER_SIZE] = {0};
        const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        bind(server_fd, (struct sockaddr *) &address, sizeof(address));
        listen(server_fd, 3);



        printf(">>>!\n");
        const int m_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
        printf(">>>1!\n");
        read(m_socket, buffer, BUFFER_SIZE);
        printf(">>>2!\n");
        if (strlen(buffer) > 0) {
            log_info_string(TAG, "REQUEST:", buffer);
            char *response = this->handle_request(buffer);
            log_info_string(TAG, "RESPONSE:", response);
            send(m_socket, ACCEPT_STATUS, ACCEPT_STATUS_SIZE, 0);
            bzero(buffer, BUFFER_SIZE);
            delete response;
        }
        printf(">>>3!\n");
    }
}

char *m_middle_server::handle_request(const char *request)
{
    return new_string(ACCEPT_STATUS);
}
