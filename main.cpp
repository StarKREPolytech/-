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
#include <csignal>
#include <util/common.h>
#include <o_client.h>
#include <o_middle_server.h>
#include <o_left_server.h>
#include <o_right_server.h>

int main() {

    //Open named pipes:
    mknod(CHANNEL_1_NAME, S_IFIFO | 0666, 0);
    mknod(CHANNEL_2_NAME, S_IFIFO | 0666, 0);

    //Open anonymous pipes:
    const int *const middle_left = create_anonymous_pipeline();
    const int *const middle_right = create_anonymous_pipeline();
    const int *const left_right = create_anonymous_pipeline();
    const int *const left_middle = create_anonymous_pipeline();
    const int *const right_left = create_anonymous_pipeline();
    const int *const right_middle = create_anonymous_pipeline();

    //Configure system:
    o_client *client = new o_client();
    o_middle_server *middle_server = new o_middle_server();
    middle_server->input_left_anonymous_gate = left_middle[0];
    middle_server->output_left_anonymous_gate = middle_left[1];
    middle_server->input_right_anonymous_gate = right_middle[0];
    middle_server->output_right_anonymous_gate = middle_right[1];
    o_left_server *left_server = new o_left_server();
    left_server->input_right_anonymous_gate = right_left[0];
    left_server->output_right_anonymous_gate = left_right[1];
    left_server->middle_server_input_channel = middle_left[0];
    left_server->middle_server_output_channel = left_middle[1];
    o_right_server *right_server = new o_right_server();
    right_server->input_middle_anonymous_gate = middle_right[0];
    right_server->output_middle_anonymous_gate = right_middle[1];
    right_server->left_server_input_channel = left_right[0];
    right_server->left_server_output_channel = right_left[1];

    //Launch processes:
    if (fork() == 0) {
        if (fork() == 0) {
            if (fork() == 0) {
                left_server->start();
            } else {
                right_server->start();
            }
        } else {
            client->start();
        }
    } else {
        middle_server->start();
    }
}