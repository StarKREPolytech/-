#include <o_right_server.h>
#include <cstring>
#include <zconf.h>
#include <string>
#include <fstream>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <lib4aio/lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio/lib4aio_cpp_headers/utils/file_utils/file_reader.h>
#include <util/common.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_RIGHT_SERVER"

#define PATH "../io/right_server/sample1.txt"

void right_server::start()
{
    while (true) {
        char left_server_request[BUFFER_SIZE] = {0};
        while (strlen(left_server_request) == 0) {
            read(this->left_server_input_channel, left_server_request, BUFFER_SIZE);
        }
        const long last_modified_date = get_file_last_modified_time(PATH);
        str_hook *left_server_request_hook = new str_hook(left_server_request);
        str_hook_list *chunks = left_server_request_hook->split_by_comma();
        char *left_server_content = chunks->get(0)->to_string();
        char *left_server_date_str = chunks->get(1)->to_string();
        string::size_type type;
        const long left_server_last_modified_time = stol(left_server_date_str, &type);
        if (last_modified_date <= left_server_last_modified_time ) {
            ofstream file;
            file.open(PATH);
            file << left_server_content;
            file.close();
            system("tar -czvf ../io/right_server/sample1.tar.gz ../io/right_server/sample1.txt");
            log_info_string(TAG, "ARCHIVED DATA!", left_server_content);
        }
        bzero(left_server_request, BUFFER_SIZE);
        write(this->left_server_output_channel, ACCEPT_STATUS, 7);

        str_builder *builder = read_file_by_str_builder(PATH);
        builder->append(',');
        builder->append(to_string(get_file_last_modified_time(PATH)).c_str());
        const size_t request_data_size = builder->size();
        const char *request_data = builder->pop();
        write(this->output_middle_anonymous_gate, request_data, request_data_size);
        char right_response[BUFFER_SIZE] = {0};
        while (strlen(right_response) == 0) {
            read(this->input_middle_anonymous_gate, right_response, BUFFER_SIZE);
        }
        if (strcmp(right_response, ACCEPT_STATUS)) {
            log_info(TAG, "OK!");
        }
    }
}