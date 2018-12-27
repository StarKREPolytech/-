#include <o_client.h>
#include <sys/socket.h>
#include <cstring>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/common.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <util/common.h>
#include <iostream>
#include <unistd.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_CLIENT"

void o_client::start()
{
    log_info(TAG, "START!");
    this->output_channel = open(CHANNEL_1_NAME, O_WRONLY);
    this->input_channel = open(CHANNEL_2_NAME, O_RDONLY);
    sleep(5);
    log_info(TAG, "ASK!");
    this->ask();
}

void o_client::ask()
{
    //Send last time:
    int m_socket = socket(AF_INET, SOCK_STREAM, 0);
    this->call_by_socket(GET_REQUEST, 5, m_socket);
    const char *socket_response = this->receive_by_socket(m_socket);
    log_info_string(TAG, "SERVER RESPONSE", socket_response);

    //Check status:
    if (strcmp(socket_response, ACCEPT_STATUS) == 0) {

        //Wait response:
        char channel_response[BUFFER_SIZE] = {0};
        while (strlen(channel_response) == 0) {
            read(this->input_channel, channel_response, BUFFER_SIZE);
            log_info_string(TAG, "SERVER_RESPONSE: ", channel_response);
        }
        printf("\n%s\n", channel_response);

        //Send response:
        write(this->output_channel, ACCEPT_STATUS, BUFFER_SIZE);
        bzero(channel_response, BUFFER_SIZE);
    }
    delete socket_response;
}

void o_client::call_by_socket(const char *request, const unsigned long size, const int m_socket)
{
    sockaddr_in server_address;
    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, ADDRESS, &server_address.sin_addr);
    connect(m_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    send(m_socket, request, size, 0);
    log_info_string(TAG, "SOCKET_REQUEST:", request);
}

char *o_client::receive_by_socket(const int m_socket)
{
    char response[BUFFER_SIZE] = {0};
    while (strlen(response) == 0) {
        read(m_socket, response, BUFFER_SIZE);
    }
    log_info_string(TAG, "SOCKET_RESPONSE:", response);
    char *result = new_string(response);
    bzero(response, BUFFER_SIZE);
    return result;
}