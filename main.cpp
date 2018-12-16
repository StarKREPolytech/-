#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <ctime>
#include <iostream>

//void* create_shared_memory(const size_t size) {
//    int protection = PROT_READ | PROT_WRITE;
//    int visibility = MAP_ANONYMOUS | MAP_SHARED;
//    return mmap(nullptr, size, protection, visibility, 0, 0);
//}
//
//int main() {
//
//
//
//    char* parent_message = const_cast<char *>("hello");  // parent process will write this message
//    char* child_message = const_cast<char *>("goodbye"); // child process will then write this one
//
//    void* shmem = create_shared_memory(2048);
//
//    memcpy(shmem, parent_message, sizeof(parent_message));
//
//    const int pid = fork();
//
//    if (pid == 0) {
//        printf("Child read: %s\n", shmem);
//        memcpy(shmem, child_message, sizeof(child_message));
//        printf("Child wrote: %s\n", shmem);
//
//    } else {
//        printf("Parent read: %s\n", shmem);
//        sleep(1);
//        printf("After 1s, parent read: %s\n", shmem);
//    }
//}

static const char *const TARGET_FILE_PATH = "../io/sample1.txt";

class file_archiver {

public:

    explicit file_archiver(const int read_gate)
    {
        this->read_gate = read_gate;
    }

    void start() const
    {
        printf("AAAA\n");
        char buffer[101];
        int pointer = 0;
        while (true) {
            bzero(buffer, 100);
            pointer = read(this->read_gate, buffer, 100);
            printf("HUA: %s\n", buffer);
            FILE *output_file = fopen("../io/sample2.txt", "w");
            fwrite(buffer, sizeof(char), pointer, output_file);
            fclose(output_file);
            //TODO ZIP:
        }
    }

private:

    int read_gate;
};

class file_listener {

public:

    explicit file_listener(const int write_gate)
    {
        this->write_gate = write_gate;
    }

    virtual ~file_listener() = default;

    void start() const
    {
        printf("GGG \n");
        while (true) {
            sleep(10);
            printf("HELLO I LISTENER\n");
            bool has_file_changed = this->last_modified_file
                                    < this->get_file_last_modified_time(TARGET_FILE_PATH);
            if (has_file_changed) {
                printf("HOBA!!!\n");
                FILE *target_file = fopen(TARGET_FILE_PATH, "r");
                size_t pointer = 0;
                char buffer[256];
                while (!feof(target_file)) {
                    pointer = fread(buffer, sizeof(char), 256, target_file);

                    printf("FL BUFFER: %s\n", buffer);

                    write(this->write_gate, buffer, pointer);
                }
            }
        }
    }

private:

    long last_modified_file = get_file_last_modified_time(TARGET_FILE_PATH);

    int write_gate = 0;

    static long get_file_last_modified_time(const char *path)
    {
        struct stat attr{};
        stat(path, &attr);
        return attr.st_mtime;
    }
};


int *create_anonymous_pipeline()
{
    int *file_descriptor_pipeline = static_cast<int *>(calloc(2, sizeof(int)));
    pipe(file_descriptor_pipeline);
    return file_descriptor_pipeline;
}

int main()
{
    const int *anonymous_pipeline = create_anonymous_pipeline();
    file_listener *listener = new file_listener(anonymous_pipeline[1]);
    file_archiver *archiver = new file_archiver(anonymous_pipeline[0]);
    if (fork() == 0) {
        archiver->start();
        printf("Hi: \n");
    } else {
        listener->start();

        printf("Bye: \n");
    }
}