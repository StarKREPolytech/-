#ifndef COMMON_H
#define COMMON_H

namespace lib4aio {

    class str_hook_list;
}

using namespace lib4aio;

#define PORT 8080

#define ADDRESS "127.0.0.1"

#define BUFFER_SIZE 1024

/**
 * Interaction.
 */

#define GET_REQUEST "GET"

#define ACCEPT_STATUS "ACCEPT"

#define IS_SYNC "IS_SYNC"

#define START_SYNC "START_SYNC"

/**
 * Client-server.
 */

#define CHANNEL_1_NAME "channel_1"

#define CHANNEL_2_NAME "channel_2"

int *create_anonymous_pipeline();

bool add_to_list(char **list_1, const char *list_2);

#endif //COMMON_H