
#include <old/m_server.h>
#include <zconf.h>
#include <cstdio>
#include <netinet/in.h>
#include <cstring>
#include <lib4aio/lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <util/common.h>
#include <csignal>
#include <old/m_checker.h>
#include <fstream>

#define BUFFER_SIZE 1024

#define PORT 8080

#define ACCEPT_STATUS "ACCEPT"

#define REJECT_STATUS "REJECT"

#define STATUS_SIZE 7

#define TAG "M_MIDDLE_SERVER"

using namespace lib4aio;

using namespace std;

static const char *const REQUEST_CHANNEL_NAME = "server_request_channel";

static const char *const RESPONSE_CHANNEL_NAME = "server_response_channel";

static const char *const PATH = "../io/middle_server/sample1.txt";

m_server::m_server()
{}

m_server::~m_server()
{

}

void m_server::start()
{
    log_info(TAG, "START!");
    //Create channels:
    this->request_channel = open(REQUEST_CHANNEL_NAME, O_RDONLY);
    this->response_channel = open(RESPONSE_CHANNEL_NAME, O_WRONLY);
    //Launch service:
    if (fork() == 0) {
        this->launch_service();
    } else {
        this->listen_file();
    }
}

void m_server::launch_service()
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
            log_info_string(TAG, "SYNC_REQUEST:", buffer);
            char *response = this->handle_request(buffer);
            log_info_string(TAG, "RESPONSE:", response);
            send(m_socket, ACCEPT_STATUS, STATUS_SIZE, 0);
            bzero(buffer, BUFFER_SIZE);

            //Permission granted!
            if (strcmp(response, ACCEPT_STATUS) == 0) {

                //Read file by input channel:
                while (strlen(buffer) == 0) {
                    read(this->request_channel, buffer, BUFFER_SIZE);
                    log_info_string(TAG, "INPUT_CHANNEL_FILE: ", buffer);
                }

                //Send data to archiver:
                const char *file_content = new_string(buffer);
                write(this->output_archive_channel, file_content, BUFFER_SIZE);
                bzero(buffer, BUFFER_SIZE);
                while (strlen(buffer) == 0) {
                    read(this->input_archive_channel, buffer, BUFFER_SIZE);
                }

                //Check status:
                const char *status = buffer;
                if (strcmp(status, ACCEPT_STATUS) == 0) {
                    ofstream file;
                    file.open(PATH);
                    file << file_content;
                    file.close();
                }

                //Write response to client:
                write(this->response_channel, status, BUFFER_SIZE);
                log_info_string(TAG, "OUTPUT_CHANNEL_FILE: ", ACCEPT_STATUS);
                bzero(buffer, BUFFER_SIZE);

                //GC:
                delete file_content;
            }
            delete response;
        }
        printf(">>>3!\n");
    }
}

static const char *status = nullptr;

void on_successful_last_modified_time(int signal_value)
{
    printf("ACCEPT SIGNAL: %i\n", signal_value);
    status = ACCEPT_STATUS;
    signal(signal_value, SIG_DFL);
}

void on_failed_last_modified_time(int signal_value)
{
    printf("REJECT SIGNAL: %i\n", signal_value);
    status = REJECT_STATUS;
    signal(signal_value, SIG_DFL);
}

char *m_server::handle_request(const char *request)
{
    signal(SIGUSR1, on_successful_last_modified_time);
    signal(SIGUSR2, on_failed_last_modified_time);

    //Check last modified data:
    if (fork() == 0) {
        const int flag = check_last_modified_time(
                this->input_anonymous_gate,
                this->output_anonymous_gate,
                request
        );
        kill(getppid(), flag);
        log_info(TAG, "KILLED PROCESS!");
        exit(1);
    }
    log_info(TAG, "PAUSE!");
    pause();
    log_info(TAG, "RESUME!");
    return new_string(status);
}

void m_server::set_input_anonymous_gate(int input_anonymous_gate)
{
    this->input_anonymous_gate = input_anonymous_gate;
}

void m_server::set_output_anonymous_gate(int output_anonymous_gate)
{
    this->output_anonymous_gate = output_anonymous_gate;
}

void m_server::listen_file()
{

}

//void m_server::listen_file()
//{
//    while(true) {
//        const long file_last_modified_time = get_file_last_modified_time(TARGET_FILE_PATH);
//        bool has_file_changed = this->last_modified_file < file_last_modified_time;
//        if (has_file_changed) {
//            this->last_modified_file = file_last_modified_time;
//
//            //Send last time:
//            const string last_time = to_string(this->last_modified_file);
//            int m_socket = socket(AF_INET, SOCK_STREAM, 0);
//            this->call_by_socket(last_time.c_str(), last_time.size(), m_socket);
//
//            //Receive response:
//            const char *response = this->receive_by_socket(m_socket);
//            const bool is_successful = strcmp(response, ACCEPT_STATUS) == 0;
//            if (is_successful) {
//                this->call_by_named_channel();
//            }
//            delete response;
//        }
//    }
//}