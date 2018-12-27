#ifndef COMMON_H
#define COMMON_H

#define PORT 8080

#define ADDRESS "127.0.0.1"

#define BUFFER_SIZE 1024

#define ACCEPT_STATUS "ACCEPT"

/**
 * Interaction.
 */

#define SYNC_REQUEST "SYNC"

#define START_SYNC "START SYNC"

#define IS_SYNCING "IS_SYNCING"


/**
 * Client-server.
 */

#define INPUT_CHANNEL_NAME "server_input_channel"

#define OUTPUT_CHANNEL_NAME "server_output_channel"


long get_file_last_modified_time(const char *path);

int *create_anonymous_pipeline();

#endif //COMMON_H