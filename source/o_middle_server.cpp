
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
#include <chrono>
#include <sys/time.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_MIDDLE_SERVER"

#define PATH "../io/middle_server/programs.txt"

void o_middle_server::start()
{
    log_info(TAG, "START!");

    //Create channels:
    this->input_client_channel = open(CHANNEL_1_NAME, O_RDONLY);
    this->output_client_channel = open(CHANNEL_2_NAME, O_WRONLY);

    //Launch service:
    this->launch_service();
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
        if (strcmp(buffer, MAKE_REQUEST) == 0) {
            log_info_string(TAG, "MAKE REQUEST:", buffer);
            if (!this->is_making) {
                send(m_socket, ACCEPT_STATUS, 32, 0);

                //Receive program:
                this->is_making = true;
                this->receive_program();
            } else {
                send(m_socket, REJECT_STATUS, 32, 0);
            }
            bzero(buffer, BUFFER_SIZE);
        }
        printf(">>>3!\n");
    }
}

using namespace std::chrono;

void o_middle_server::receive_program()
{

    //Receive program:
    char program[BUFFER_SIZE] = {0};
    while (strlen(program) == 0) {
        read(this->input_client_channel, program, BUFFER_SIZE);
    }
    log_info_string(TAG, "PROGRAM!", program);

    timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = static_cast<int>(curTime.tv_usec / 1000);

    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

    char currentTime[84] = "";
    sprintf(currentTime, "%s:%d", buffer, milli);
    printf("current time: %s \n", currentTime);


    //Save file:
    ofstream file;
    file.open(PATH);
    file
            << currentTime << "\n"
            << strlen(program) * sizeof(char) << "bytes\n"
            << program;
    file.close();

    while (this->is_making) {
        //Send program to the left server:
        write(this->output_left_anonymous_gate, program, BUFFER_SIZE);
        char left_response[BUFFER_SIZE] = {0};
        while (strlen(left_response) == 0) {
            read(this->input_left_anonymous_gate, left_response, BUFFER_SIZE);
            log_info_string(TAG, "LEFT RESPONSE:", left_response);
        }
        if (strcmp(left_response, ACCEPT_STATUS) == 0) {
            log_info(TAG, "SEND PROGRAM TO THE LEFT SERVER!");
            char right_request[BUFFER_SIZE] = {0};
            while (strlen(right_request) == 0) {
                read(this->input_right_anonymous_gate, right_request, BUFFER_SIZE);
                log_info_string(TAG, "RIGHT PROGRAM:", left_response);
            }
            if (strcmp(right_request, program) == 0) {
                write(this->output_right_anonymous_gate, ACCEPT_STATUS, 7);
                write(this->output_client_channel, ACCEPT_STATUS, 7);
                this->is_making = false;
            }
            bzero(right_request, BUFFER_SIZE);
        }
        bzero(left_response, BUFFER_SIZE);
        bzero(program, BUFFER_SIZE);
    }
}