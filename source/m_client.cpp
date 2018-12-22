#include <m_client.h>
#include <cstdio>
#include <sys/stat.h>
#include <zconf.h>
#include <lib4aio/lib4aio_cpp_headers/utils/file_utils/file_reader.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/common.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <functional>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/cast.h>
#include <sstream>
#include <fcntl.h>
#include <util/common.h>

#define TARGET_FILE_PATH "../io/client/sample1.txt"

#define PORT 8080

#define ADDRESS "127.0.0.1"

#define BUFFER_SIZE 1024

#define ACCEPT_STATUS "ACCEPT"

using namespace lib4aio;

using namespace std;

#define TAG "M_CLIENT"

static const char *const REQUEST_CHANNEL_NAME = "server_request_channel";

static const char *const RESPONSE_CHANNEL_NAME = "server_response_channel";

m_client::m_client()
{
    this->last_modified_file = get_file_last_modified_time(TARGET_FILE_PATH);
}

m_client::~m_client() = default;

void m_client::start()
{
    printf("m_client has started!\n");
    this->output_channel = open(REQUEST_CHANNEL_NAME, O_WRONLY);
    this->input_channel = open(RESPONSE_CHANNEL_NAME, O_RDONLY);
    while (true) {
        sleep(1);
        log_info(TAG, "LISTEN");
        this->listen_file();
    }
}

void m_client::listen_file()
{
    const long file_last_modified_time = get_file_last_modified_time(TARGET_FILE_PATH);
    bool has_file_changed = this->last_modified_file < file_last_modified_time;
    if (has_file_changed) {
        this->last_modified_file = file_last_modified_time;

        //Send last time:
        const string last_time = to_string(this->last_modified_file);
        int m_socket = socket(AF_INET, SOCK_STREAM, 0);
        this->call_by_socket(last_time.c_str(), last_time.size(), m_socket);

        //Receive response:
        const char *file_response = this->receive_by_socket(m_socket);
        const bool is_successful = strcmp(file_response, ACCEPT_STATUS) == 0;
        if (is_successful) {
            this->call_by_named_channel();
        }
        delete file_response;
    }
}

void m_client::call_by_socket(const char *request, const unsigned long size, const int m_socket)
{
    sockaddr_in server_address;
    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, ADDRESS, &server_address.sin_addr);
    connect(m_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    send(m_socket, request, size, 0);
    log_info_string(TAG, "REQUEST:", request);
}

char *m_client::receive_by_socket(const int m_socket)
{
    char response[BUFFER_SIZE] = {0};
    while (strlen(response) == 0) {
        read(m_socket, response, BUFFER_SIZE);
    }
    log_info_string(TAG, "RESPONSE:", response);
    char *result = new_string(response);
    bzero(response, BUFFER_SIZE);
    return result;
}

void m_client::call_by_named_channel()
{
    //Send file content:
    str_builder *file_builder = read_file_by_str_builder(TARGET_FILE_PATH);
    const unsigned file_content_size = file_builder->size();
    const char *file_content = file_builder->pop();
    write(this->output_channel, file_content, file_content_size);
    char response[BUFFER_SIZE] = {0};
    while (strlen(response) == 0) {
        read(this->input_channel, response, BUFFER_SIZE);
        log_info_string(TAG, "CLIENT_FILE_RESPONSE: ", response);
    }
    bzero(response, BUFFER_SIZE);
    //Delete:
    delete file_builder;
    delete file_content;
}
