#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <m_client.h>
#include <m_server.h>
#include <csignal>
#include <util/common.h>
#include <m_archiver.h>

static const char *const REQUEST_CHANNEL_NAME = "server_request_channel";

static const char *const RESPONSE_CHANNEL_NAME = "server_response_channel";

int main()
{
    //Open anonymous pipe:
    const int *checker_to_archiver_pipe = create_anonymous_pipeline();
    const int *archiver_to_checker_pipe = create_anonymous_pipeline();
    const int *server_to_archiver_pipe = create_anonymous_pipeline();
    const int *archiver_to_server_pipe = create_anonymous_pipeline();

    //Open named pipes:
    mknod(REQUEST_CHANNEL_NAME, S_IFIFO | 0666, 0);
    mknod(RESPONSE_CHANNEL_NAME, S_IFIFO | 0666, 0);
    mknod(ARCHIVE_CHANNEL_NAME_1, S_IFIFO | 0666, 0);
    mknod(ARCHIVE_CHANNEL_NAME_2, S_IFIFO | 0666, 0);


//    const int output_server_channel = open(ARCHIVE_CHANNEL_NAME_1, O_WRONLY);
//    const int input_server_channel = open(ARCHIVE_CHANNEL_NAME_2, O_RDONLY);

    //Create client:
    m_client *client = new m_client();
    //Create server system:
    m_server *server = new m_server();
    server->set_input_anonymous_gate(archiver_to_checker_pipe[0]);
    server->set_output_anonymous_gate(checker_to_archiver_pipe[1]);
    server->input_archive_channel = archiver_to_server_pipe[0];
    server->output_archive_channel = server_to_archiver_pipe[1];
    m_archiver *archiver = new m_archiver();
    archiver->set_input_anonymous_gate(checker_to_archiver_pipe[0]);
    archiver->set_output_anonymous_gate(archiver_to_checker_pipe[1]);
    archiver->input_server_channel = server_to_archiver_pipe[0];
    archiver->output_server_channel = archiver_to_server_pipe[1];
    //Launch processes:
    if (fork() == 0) {
        if (fork() == 0) {
            server->start();
        } else {
            client->start();
        }
    } else {
        archiver->start();
    }
}