#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

#define TARGET_FILE_PATH "../io/sample1.txt"

#define CHANNEL_NAME "ARCHIVE_CHANNEL"

void *create_shared_memory(const size_t size)
{
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(nullptr, size, protection, visibility, 0, 0);
}

class file_archiver {

public:

    explicit file_archiver(const int read_gate, const int named_channel)
    {
        this->read_gate = read_gate;
        this->named_channel = named_channel;
    }

    void start() const
    {
        printf("Archiver has started!\n");
        char buffer[101];
        int pointer = 0;
        while (true) {
            bzero(buffer, 100);
            pointer = read(this->read_gate, buffer, 100);
            FILE *output_file = fopen("../io/sample2.txt", "w");
            fwrite(buffer, sizeof(char), pointer, output_file);
            fclose(output_file);
            //TODO ZIP:
            write(this->named_channel, buffer, 100);
        }
    }


private:

    int read_gate;

    int named_channel = 0;
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
        printf("Listener has started!\n");
        while (true) {
            sleep(10);
            bool has_file_changed = this->last_modified_file
                                    < this->get_file_last_modified_time(TARGET_FILE_PATH);
            if (has_file_changed) {
                FILE *target_file = fopen(TARGET_FILE_PATH, "r");
                size_t pointer = 0;
                char buffer[256];
                while (!feof(target_file)) {
                    pointer = fread(buffer, sizeof(char), 256, target_file);
                    //Notify archiver:
                    write(this->write_gate, buffer, pointer);
                    //Change shared memory:
                    memcpy(this->shared_memory_ptr, buffer, sizeof(buffer));
                }
            }
        }
    }

    void set_shared_memory_ptr(void *shared_memory_ptr)
    {
        this->shared_memory_ptr = shared_memory_ptr;
    }

private:

    long last_modified_file = get_file_last_modified_time(TARGET_FILE_PATH);

    int write_gate = 0;

    void *shared_memory_ptr = nullptr;

    static long get_file_last_modified_time(const char *path)
    {
        struct stat attr{};
        stat(path, &attr);
        return attr.st_mtime;
    }
};

class file_visualizer {

public:

    explicit file_visualizer(const int named_channel)
    {
        this->named_channel = named_channel;
    }

    virtual ~file_visualizer() = default;

    void set_shared_memory_ptr(void *shared_memory_ptr)
    {
        this->shared_memory_ptr = shared_memory_ptr;
    }

    void start() const
    {
        printf("Visualizer has started!\n");
        int pointer = 0;
        char buffer[101];
        while (true) {
            sleep(7);
            printf("Personal data: %s\n", static_cast<char *>(this->shared_memory_ptr));
            bzero(buffer, 100);
            read(this->named_channel, buffer, 100);
            printf("Archived personal data: <%s>\n", buffer);
            struct sockaddr_in server_address;
            const int sock = socket(AF_INET, SOCK_STREAM, 0);
            memset(&server_address, '0', sizeof(server_address));
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(PORT);
            inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
            connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
            send(sock, buffer, 100, 0);
            bzero(buffer, 100);
            read(sock, buffer, 1024);
            printf("Response from server: <%s>\n", buffer);
        }
    }

private:

    void *shared_memory_ptr = nullptr;

    int named_channel = 0;
};

class file_server {

public:

    explicit file_server()
    {}

    virtual ~file_server()
    {}

    void start() const
    {
        printf("Server has started!\n");
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};

        const int server_fd = socket(AF_INET, SOCK_STREAM, 0);

        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        bind(server_fd, (struct sockaddr *) &address, sizeof(address));
        listen(server_fd, 1000);

        const int new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);

        read(new_socket, buffer, 1024);
        const unsigned space_count = this->get_space_count(buffer);
        const char *result = int_to_string(space_count);
        printf("Server result: %s\n", result);
        send(new_socket, result, 10, 0);
    }


private:

    unsigned get_space_count(const char *string) const
    {
        unsigned space_counter = 0;
        const unsigned str_length = strlen(string);
        for (unsigned i = 0; i < str_length; ++i) {
            const char symbol = string[i];
            if (symbol == ' ') {
                space_counter++;
            }
        }
        return space_counter;
    }


    char *int_to_string(const int src) const
    {
        auto division = src;
        unsigned int_size_in_string = 0;
        while (division != 0) {
            division = division / 10;
            int_size_in_string = int_size_in_string + 1;
        }
        char *integer_array;
        auto negative_shift = 0;
        if (int_size_in_string > 0) {
            if (src < 0) {
                negative_shift = 1;
            }
            integer_array = (char *) calloc(int_size_in_string + 1 + negative_shift, sizeof(char));
            division = src;
            integer_array[0] = '-';
            auto pointer = int_size_in_string - 1 + negative_shift;
            while (division != 0) {
                (integer_array)[pointer] = (char) (abs(division % 10) + '0');
                division = division / 10;
                pointer--;
            }
        } else {
            integer_array = (char *) calloc(2, sizeof(char));
            integer_array[0] = '0';
            int_size_in_string = 1;
        }
        char *dst = (char *) calloc(int_size_in_string + 1 + negative_shift, sizeof(char));
        for (unsigned k = 0; k < int_size_in_string + negative_shift; ++k) {
            dst[k] = integer_array[k];
        }
        free(integer_array);
        return dst;
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
    const int read_gate = anonymous_pipeline[0];
    const int write_gate = anonymous_pipeline[1];
    mknod(CHANNEL_NAME, S_IFIFO | 0666, 0);
    const int named_channel = open(CHANNEL_NAME, O_RDWR);
    //Components:
    const file_archiver *archiver = new file_archiver(read_gate, named_channel);
    const file_server *server = new file_server();
    file_listener *listener = new file_listener(write_gate);
    file_visualizer *visualizer = new file_visualizer(named_channel);
    //Logic:
    if (fork() == 0) {
        archiver->start();
    } else {
        void *shared_memory = create_shared_memory(1024);
        listener->set_shared_memory_ptr(shared_memory);
        visualizer->set_shared_memory_ptr(shared_memory);
        if (fork() == 0) {
            visualizer->start();
        } else {
            if (fork() == 0) {
                server->start();
            } else {
                listener->start();
            }
        }
    }
}