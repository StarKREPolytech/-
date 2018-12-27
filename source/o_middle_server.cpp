#include <o_middle_server.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <util/common.h>
#include <cstdio>
#include <cstring>
#include <strings.h>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/common.h>
#include <fstream>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio/lib4aio_cpp_headers/utils/file_utils/file_reader.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <unistd.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_MIDDLE_SERVER"

#define PATH "../io/middle_server/sample1.txt"

void o_middle_server::start()
{
    log_info(TAG, "START!");
    //Create channels:
    this->input_client_channel = open(CHANNEL_1_NAME, O_RDONLY);
    this->output_client_channel = open(CHANNEL_2_NAME, O_WRONLY);
    //Launch service:
    if (fork() == 0) {
        log_info(TAG, "START LISTENING");
        this->listen_file();
    } else {
        log_info(TAG, "START SERVICE");
        this->launch_service();
    }
}

void o_middle_server::launch_service()
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
            log_info_string(TAG, "GET_REQUEST:", buffer);
            if (strcmp(buffer, GET_REQUEST) == 0) {
                if (this->is_syncing) {
                    send(m_socket, IS_SYNC, 32, 0);
                } else {
                    send(m_socket, START_SYNC, 32, 0);
                    this->sync();
                }
                bzero(buffer, BUFFER_SIZE);
            }
        }
        printf(">>>3!\n");
    }
}

void o_middle_server::listen_file()
{
    while (true) {
        sleep(10);
        this->sync();
    }
}

void o_middle_server::sync()
{
    //Load file:
    str_builder *builder = read_file_by_str_builder(PATH);
    const size_t size = builder->size();
    char *source_list = builder->pop();

    //Start sync:
    log_info_string(TAG, "COMPOSED DATA", source_list);
    this->is_syncing = true;
    while (this->is_syncing) {
        log_info(TAG, "START_SYNC");

        //Send data:
        write(this->output_left_anonymous_gate, source_list, size);
        char left_response[BUFFER_SIZE] = {0};
        while (strlen(left_response) == 0) {
            read(this->input_left_anonymous_gate, left_response, BUFFER_SIZE);
        }
        if (strcmp(left_response, ACCEPT_STATUS) == 0) {

            //Wait right response:
            char right_response[BUFFER_SIZE] = {0};
            while (strlen(right_response) == 0) {
                read(this->input_right_anonymous_gate, right_response, BUFFER_SIZE);
            }
            log_info_string(TAG, "RIGHT_RESPONSE", right_response);
            if (add_to_list(&source_list, right_response)) {
                this->is_syncing = false;
                log_info(TAG, "Sync is complete");
            } else {
                ofstream file;
                file.open(PATH);
                file << right_response;
                file.close();
                log_info_string(TAG, "REFRESHED LIST!", source_list);
            }
            bzero(right_response, BUFFER_SIZE);
        }
        bzero(left_response, BUFFER_SIZE);
    }
    write(this->output_client_channel, source_list, BUFFER_SIZE);
    char client_response[BUFFER_SIZE] = {0};
    while (strlen(client_response) == 0) {
        read(this->input_client_channel, client_response, BUFFER_SIZE);
        if (strcmp(client_response, ACCEPT_STATUS) == 0) {
            log_info(TAG, "LIST WAS SENT!");
        }
    }
    bzero(client_response, BUFFER_SIZE);
}