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

#define PORT 8080

#define CHANNEL_NAME "ARCHIVE_CHANNEL"


int main()
{
    //Create client:
    m_client *client = new m_client();
    //Create server system:
    m_server *middle_server = new m_server();
    m_server *left_server = new m_server();
    m_server *right_server = new m_server();
    if (fork() == 0) {
        left_server->start();
    } else {
        if (fork() == 0) {
            right_server->start();
        } else {
            if (fork() == 0) {
                middle_server->start();
            } else {
                client->start();
            }
        }
    }

}

//int main() {
//    const int *anonymous_pipeline = create_anonymous_pipeline();
//    const int read_gate = anonymous_pipeline[0];
//    const int write_gate = anonymous_pipeline[1];
//    mknod(CHANNEL_NAME, S_IFIFO | 0666, 0);
//    const int named_channel = open(CHANNEL_NAME, O_RDWR);
//    //Components:
//    const file_archiver *archiver = new file_archiver(read_gate, named_channel);
//    const file_server *server = new file_server();
//    file_listener *listener = new file_listener(write_gate);
//    file_visualizer *visualizer = new file_visualizer(named_channel);
//    //Logic:
//    if (fork() == 0) {
//        archiver->start();
//    } else {
//        void *shared_memory = create_shared_memory(BUFFER_SIZE);
//        listener->set_shared_memory_ptr(shared_memory);
//        visualizer->set_shared_memory_ptr(shared_memory);
//        if (fork() == 0) {
//            visualizer->start();
//        } else {
//            if (fork() == 0) {
//                server->start();
//            } else {
//                listener->start();
//            }
//        }
//    }
//}