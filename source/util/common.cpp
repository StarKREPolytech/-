#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>

long get_file_last_modified_time(const char *path)
{
    struct stat attr{};
    stat(path, &attr);
    return attr.st_mtime;
}

int *create_anonymous_pipeline()
{
    int *file_descriptor_pipeline = static_cast<int *>(calloc(2, sizeof(int)));
    pipe(file_descriptor_pipeline);
    return file_descriptor_pipeline;
}