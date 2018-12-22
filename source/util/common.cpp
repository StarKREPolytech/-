#include <sys/stat.h>

long get_file_last_modified_time(const char *path)
{
    struct stat attr{};
    stat(path, &attr);
    return attr.st_mtime;
}