
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

#define PATH "../io/middle_server/sample1.c"

void o_middle_server::start() {
    log_info(TAG, "START!");

    //Create channels:
    this->input_client_channel = open(CHANNEL_1_NAME, O_RDONLY);
    this->output_client_channel = open(CHANNEL_2_NAME, O_WRONLY);

    //Launch service:
    this->launch_service();
}

void o_middle_server::launch_service() {

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
        if (strcmp(buffer, MAKE_REQUEST) == 0) {
            log_info_string(TAG, "MAKE REQUEST:", buffer);
            if (this->is_syncing) {
                send(m_socket, ACCEPT_STATUS, 32, 0);

                //Receive program:
                this->receive_program();
            } else {
                send(m_socket, REJECT_STATUS, 32, 0);
            }
            bzero(buffer, BUFFER_SIZE);
        }
        printf(">>>3!\n");
    }
}

void o_middle_server::receive_program() {
    char program[BUFFER_SIZE] = {0};
    while (strlen(program) == 0) {
        read(this->input_client_channel, program, BUFFER_SIZE);
    }
    log_info_string(TAG, "PROGRAM!", program);
    ofstream file;
    file.open(PATH);
    file << program;
    file.close();
    system("gcc  ../io/middle_server/sample1.c -o ../io/middle_server/sample1");
    bzero(program, BUFFER_SIZE);
}

void o_middle_server::sync() {
    this->is_syncing = true;
    while (this->is_syncing) {
        log_info(TAG, "START_SYNCING");
        const long request_last_modified_date = get_file_last_modified_time(PATH);
        str_builder *builder = read_file_by_str_builder(PATH);
        builder->append(',');
        builder->append(to_string(request_last_modified_date).c_str());
        const size_t request_data_size = builder->size();
        const char *request_data = builder->pop();
        log_info_string(TAG, "COMPOSED DATA", request_data);
        write(this->output_left_anonymous_gate, request_data, request_data_size);
        char left_response[BUFFER_SIZE] = {0};
        while (strlen(left_response) == 0) {
            read(this->input_left_anonymous_gate, left_response, BUFFER_SIZE);
        }
        if (strcmp(left_response, ACCEPT_STATUS) == 0) {
            char right_response[BUFFER_SIZE] = {0};
            while (strlen(right_response) == 0) {
                read(this->input_right_anonymous_gate, right_response, BUFFER_SIZE);
            }
            log_info_string(TAG, "RIGHT_RESPONSE", right_response);
            str_hook *right_response_hook = new str_hook(right_response);
            str_hook_list *chunks = right_response_hook->split_by_comma();
            char *right_content = chunks->get(0)->to_string();
            char *right_date = chunks->get(1)->to_string();
            string::size_type type;
            const long response_last_modified_time = stol(right_date, &type);
            if (response_last_modified_time <= request_last_modified_date) {
                this->is_syncing = false;
                log_info(TAG, "Sync is complete");
            } else {
                ofstream file;
                file.open(PATH);
                file << right_content;
                file.close();
                system("tar -czvf ../io/middle_server/sample1.tar.gz ../io/middle_server/sample1.c");
                log_info_string(TAG, "ARCHIVED DATA!", right_content);
            }
            bzero(right_response, BUFFER_SIZE);
        }
        bzero(left_response, BUFFER_SIZE);
    }
    write(this->output_client_channel, ACCEPT_STATUS, 7);
    char client_response[BUFFER_SIZE] = {0};
    while (strlen(client_response) == 0) {
        read(this->input_client_channel, client_response, BUFFER_SIZE);
        if (strcmp(client_response, ACCEPT_STATUS) == 0) {
            log_info(TAG, "SYNC WAS SUCCESSFUL!");
        }
    }
    bzero(client_response, BUFFER_SIZE);
}