
#include <m_archiver.h>
#include <fcntl.h>
#include <cstring>
#include <zconf.h>
#include <fstream>
#include <util/common.h>

using namespace std;

#define BUFFER_SIZE 1024

static const char *const INPUT_SERVER_CHANNEL_NAME = "archive_channel_2";

static const char *const OUTPUT_SERVER_CHANNEL_NAME = "archive_channel_1";

void m_archiver::start()
{
    this->input_server_channel = open(INPUT_SERVER_CHANNEL_NAME, O_RDONLY);
    this->output_server_channel = open(OUTPUT_SERVER_CHANNEL_NAME, O_WRONLY);
    while (true) {
        char buffer[BUFFER_SIZE] = {0};
        while (strlen(buffer) == 0) {
            read(this->input_anonymous_gate, buffer, BUFFER_SIZE);
        }
        string::size_type type;
        const long last_modified_archive = get_file_last_modified_time("../io/archive/sample1.txt");
        const long last_modified_from_client = stol(buffer, &type);
        if (last_modified_archive < last_modified_from_client) {
            write(this->output_anonymous_gate, "ACCEPT", 7);
        } else {
            write(this->output_anonymous_gate, "REJECT", 7);
        }
        bzero(buffer, BUFFER_SIZE);
        while (strlen(buffer) == 0){
            read(this->input_server_channel, buffer, BUFFER_SIZE);
        }
        ofstream file;
        file.open("../io/archive/sample1.txt");
        file << buffer;
        file.close();
    }
}

void m_archiver::set_input_anonymous_gate(int input_anonymous_gate)
{
    this->input_anonymous_gate = input_anonymous_gate;
}

void m_archiver::set_output_anonymous_gate(int output_anonymous_gate)
{
    this->output_anonymous_gate = output_anonymous_gate;
}
