#include <o_left_server.h>
#include <util/common.h>
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
        long last_modified_date = get_file_last_modified_time(PATH);
        str_hook *middle_server_request_hook = new str_hook(middle_server_request);
        str_hook_list *chunks = middle_server_request_hook->split_by_comma();
        char *middle_server_content = chunks->get(0)->to_string();
        char *middle_server_date_str = chunks->get(1)->to_string();
        string::size_type type;
        const long middle_server_last_modified_time = stol(middle_server_date_str, &type);
        //TODO: CREATE VARIABLE OF DATE

        if (last_modified_date <= middle_server_last_modified_time ) {
            last_modified_date = middle_server_last_modified_time;
            ofstream file;
            file.open(PATH);
            file << middle_server_content;
            file.close();
            system("tar -czvf ../io/left_server/sample1.tar.gz ../io/left_server/sample1.c");
            log_info_string(TAG, "ARCHIVED DATA!", middle_server_content);
        }
        bzero(middle_server_request, BUFFER_SIZE);
        write(this->middle_server_output_channel, ACCEPT_STATUS, 7);

        str_builder *builder = read_file_by_str_builder(PATH);
        builder->append(',');
        builder->append(to_string(last_modified_date).c_str());
        const size_t request_data_size = builder->size();
        const char *request_data = builder->pop();
        write(this->output_right_anonymous_gate, request_data, request_data_size);
        char right_response[BUFFER_SIZE] = {0};
        while (strlen(right_response) == 0) {
            read(this->input_right_anonymous_gate, right_response, BUFFER_SIZE);
        }
        if (strcmp(right_response, ACCEPT_STATUS)) {
            log_info(TAG, "OK!");
        }
    }
}