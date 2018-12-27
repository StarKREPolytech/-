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
#include <o_right_server.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_RIGHT_SERVER"

#define PATH "../io/right_server/sample1.txt"

void o_right_server::start()
{
    log_info(TAG, "START");
    while (true) {
        char left_server_request[BUFFER_SIZE] = {0};
        while (strlen(left_server_request) == 0) {
            read(this->left_server_input_channel, left_server_request, BUFFER_SIZE);
        }
        str_builder *builder = read_file_by_str_builder(PATH);
        char *source_list = builder->pop();
        if (add_to_list(&source_list, left_server_request)) {
            ofstream file;
            file.open(PATH);
            file << source_list;
            file.close();
            log_info_string(TAG, "REFRESHED LIST!", source_list);
        }
        bzero(left_server_request, BUFFER_SIZE);
        write(this->left_server_output_channel, ACCEPT_STATUS, 7);
        write(this->output_middle_anonymous_gate, source_list, BUFFER_SIZE);
        char middle_response[BUFFER_SIZE] = {0};
        while (strlen(middle_response) == 0) {
            read(this->input_middle_anonymous_gate, middle_response, BUFFER_SIZE);
        }
        if (strcmp(middle_response, ACCEPT_STATUS)) {
            log_info(TAG, "OK!");
        }
    }
}