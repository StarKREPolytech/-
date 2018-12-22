
#include <m_middle_server.h>
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

static const char *const PATH = "../io/m_middle_server/sample1.txt";

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
            send(m_socket, ACCEPT_STATUS, STATUS_SIZE, 0);
            bzero(buffer, BUFFER_SIZE);

            //Permission granted!
            if (strcmp(response, ACCEPT_STATUS) == 0) {

                //Read file by input channel:
                while (strlen(buffer) == 0) {
                    read(this->input_channel, buffer, BUFFER_SIZE);
                    log_info_string(TAG, "INPUT_CHANNEL_FILE: ", buffer);
                }
                write(this->output_channel, ACCEPT_STATUS, BUFFER_SIZE);
                log_info_string(TAG, "OUTPUT_CHANNEL_FILE: ", ACCEPT_STATUS);
                bzero(buffer, BUFFER_SIZE);
            }
            delete response;
        }
        printf(">>>3!\n");
    }
}

static const char *status = nullptr;

static bool is_locked = false;

void on_successful_last_modified_time(int signal_value)
{
    printf("ACCEPT SIGNAL: %i\n", signal_value);
    status = ACCEPT_STATUS;
    is_locked = false;
    signal(signal_value, SIG_DFL);
}

void on_failed_last_modified_time(int signal_value)
{
    printf("REJECT SIGNAL: %i\n", signal_value);
    status = REJECT_STATUS;
    is_locked = false;
    signal(signal_value, SIG_DFL);
}

#define ACCEPT_FLAG 1000

#define REJECT_FLAG 1001

char *m_middle_server::handle_request(const char *request)
{
    is_locked = true;
    signal(SIGUSR1, on_successful_last_modified_time);
    signal(SIGUSR2, on_failed_last_modified_time);

    //Check last modified data:
    if (fork() == 0) {

        //Start process:
        log_info(TAG, "STARTED CHECKER!");
        string::size_type type;
        const long last_modified_file = get_file_last_modified_time(PATH);
        const long input_last_modified_time = stol(request, &type);
        int flag;
        if (last_modified_file < input_last_modified_time) {
            flag = SIGUSR1;
            log_info(TAG, "YES!");
        } else {
            flag = SIGUSR2;
            log_info(TAG, "NO!");
        }
        kill(getppid(), flag);
        log_info(TAG, "KILLED PROCESS!");
        exit(1);
    }
    log_info(TAG, "PAUSE!");
    pause();
    log_info(TAG, "RESUME!");

    //Rewrite
    if (fork() == 0) {

    }
    pause();


    return new_string(status);
}