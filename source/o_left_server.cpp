#include <o_left_server.h>
#include <util/common.h>
#include <cstring>
#include <string>
#include <fstream>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <lib4aio/lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio/lib4aio_cpp_headers/utils/file_utils/file_reader.h>
#include <unistd.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_LEFT_SERVER"

#define PATH "../io/left_server/sample1.txt"

void o_left_server::start()
{
    log_info(TAG, "START");
    while (true) {
        char middle_server_request[BUFFER_SIZE] = {0};
        while (strlen(middle_server_request) == 0) {
            read(this->middle_server_input_channel, middle_server_request, BUFFER_SIZE);
        }
        str_builder *builder = read_file_by_str_builder(PATH);
        char *source_list = builder->pop();
        if (add_to_list(&source_list, middle_server_request)) {
            ofstream file;
            file.open(PATH);
            file << source_list;
            file.close();
            log_info_string(TAG, "REFRESHED LIST!", source_list);
        }
        bzero(middle_server_request, BUFFER_SIZE);
        write(this->middle_server_output_channel, ACCEPT_STATUS, 7);
        write(this->output_right_anonymous_gate, source_list, BUFFER_SIZE);
        char right_response[BUFFER_SIZE] = {0};
        while (strlen(right_response) == 0) {
            read(this->input_right_anonymous_gate, right_response, BUFFER_SIZE);
        }
        if (strcmp(right_response, ACCEPT_STATUS)) {
            log_info(TAG, "OK!");
        }
    }
}