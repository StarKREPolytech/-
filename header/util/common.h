#ifndef COMMON_H
#define COMMON_H

#define ARCHIVE_CHANNEL_NAME_1 "archive_channel___1"

#define ARCHIVE_CHANNEL_NAME_2 "archive_channel___2"

long get_file_last_modified_time(const char *path);

int *create_anonymous_pipeline();

#endif //COMMON_H