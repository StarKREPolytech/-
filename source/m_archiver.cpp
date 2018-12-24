
#include <m_archiver.h>
#include <fcntl.h>
#include <cstring>
#include <zconf.h>
#include <fstream>
#include <util/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>

using namespace lib4aio;

using namespace std;

#define BUFFER_SIZE 1024

#define TAG "M_ARCHIVER"

#define PATH "../io/archive/sample1.txt"

void m_archiver::start()
{
    log_info(TAG, "START!");
//    this->input_server_channel = open(ARCHIVE_CHANNEL_NAME_2, O_RDONLY);
//    this->output_server_channel = open(ARCHIVE_CHANNEL_NAME_1, O_WRONLY);
    while (true) {
        char buffer[BUFFER_SIZE] = {0};
        while (strlen(buffer) == 0) {
            read(this->input_anonymous_gate, buffer, BUFFER_SIZE);
        }
        log_info_string(TAG, "GET REQUEST FROM CHECKER", buffer);
        string::size_type type;
        const long last_modified_archive = get_file_last_modified_time(PATH);
        const long last_modified_from_client = stol(buffer, &type);
        if (last_modified_archive < last_modified_from_client) {
            log_info(TAG, "ACCEPT FOR CHECKER");
            write(this->output_anonymous_gate, "ACCEPT", 7);
        } else {
            log_info(TAG, "REJECT FOR CHECKER");
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
        write(this->output_server_channel, "ACCEPT", 7);
        system("tar -czvf ../io/archive/sample1.tar.gz ../io/archive/sample1.txt");
        log_info_string(TAG, "ARCHIVED DATA FROM SERVER", buffer);
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
