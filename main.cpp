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

static const char *const ARCHIVE_CHANNEL_NAME_1 = "archive_channel_1";

static const char *const ARCHIVE_CHANNEL_NAME_2 = "archive_channel_2";

int main()
{
    //Open anonymous pipe:
    const int *anonymous_pipeline = create_anonymous_pipeline();
    const int input_anonymous_gate = anonymous_pipeline[0];
    const int output_anonymous_gate = anonymous_pipeline[1];

    //Open named pipes:
    mknod(REQUEST_CHANNEL_NAME, S_IFIFO | 0666, 0);
    mknod(RESPONSE_CHANNEL_NAME, S_IFIFO | 0666, 0);
    mknod(ARCHIVE_CHANNEL_NAME_1, S_IFIFO | 0666, 0);
    mknod(ARCHIVE_CHANNEL_NAME_2, S_IFIFO | 0666, 0);

    //Create client:
    m_client *client = new m_client();
    //Create server system:
    m_server *server = new m_server();
    server->set_input_anonymous_gate(input_anonymous_gate);
    server->set_output_anonymous_gate(output_anonymous_gate);
    m_archiver *archiver = new m_archiver();
    archiver->set_input_anonymous_gate(input_anonymous_gate);
    archiver->set_output_anonymous_gate(output_anonymous_gate);

    //Launch processes:
    if (fork() == 0) {
        archiver->start();
    } else {
        if (fork() == 0) {
            server->start();
        } else {
            client->start();
        }
    }
}