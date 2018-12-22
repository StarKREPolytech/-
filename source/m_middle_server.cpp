
#include <m_middle_server.h>
#include <zconf.h>
#include <cstdio>
#include <netinet/in.h>
#include <cstring>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

#define PORT 8080

#define ACCEPT_STATUS "ACCEPT"

#define ACCEPT_STATUS_SIZE 7

#define TAG "M_MIDDLE_SERVER"

using namespace lib4aio;

static const char *const REQUEST_CHANNEL_NAME = "server_request_channel";

static const char *const RESPONSE_CHANNEL_NAME = "server_response_channel";

m_middle_server::m_middle_server()
{}

m_middle_server::~m_middle_server()
{

}

void m_middle_server::start()
{
    printf("m_middle_server has started!\n");

    //Create channels:
    mknod(REQUEST_CHANNEL_NAME, S_IFIFO | 0666, 0);
    mknod(RESPONSE_CHANNEL_NAME, S_IFIFO | 0666, 0);
    this->input_channel = open(REQUEST_CHANNEL_NAME, O_RDONLY);
    this->output_channel = open(RESPONSE_CHANNEL_NAME, O_WRONLY);
    //Launch service:
    this->launch_service();
}

void m_middle_server::launch_service()
{
    //Create socket:
    sockaddr_in address;
    int opt = 1;
    int address_length = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *) &address, sizeof(address));

    while (true) {
        listen(server_fd, 3);
        printf(">>>0!\n");

        //Suspend function:
        const int m_socket = accept(
                server_fd, (struct sockaddr *) &address, (socklen_t *) &address_length
        );
        printf(">>>1!\n");
        read(m_socket, buffer, BUFFER_SIZE);
        printf(">>>2!\n");

        //Buffer has read:
        if (strlen(buffer) > 0) {
            log_info_string(TAG, "REQUEST:", buffer);
            char *response = this->handle_request(buffer);
            log_info_string(TAG, "RESPONSE:", response);
            send(m_socket, ACCEPT_STATUS, ACCEPT_STATUS_SIZE, 0);
            bzero(buffer, BUFFER_SIZE);
            delete response;

            //Read file by input channel:
            while (strlen(buffer) == 0) {
                read(this->input_channel, buffer, BUFFER_SIZE);
                log_info_string(TAG, "INPUT_CHANNEL_FILE: ", buffer);
            }
            write(this->output_channel, ACCEPT_STATUS, BUFFER_SIZE);
            log_info_string(TAG, "OUTPUT_CHANNEL_FILE: ", ACCEPT_STATUS);
            bzero(buffer, BUFFER_SIZE);
        }
        printf(">>>3!\n");
    }
}

char *m_middle_server::handle_request(const char *request)
{
    return new_string(ACCEPT_STATUS);
}

