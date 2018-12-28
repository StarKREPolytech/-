#include <o_right_server.h>
#include <cstring>
#include <string>
#include <fstream>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <lib4aio/lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio/lib4aio_cpp_headers/utils/file_utils/file_reader.h>
#include <util/common.h>
#include <unistd.h>

using namespace lib4aio;

using namespace std;

#define TAG "O_RIGHT_SERVER"

#define PATH "../io/right_server/sample1.c"

void o_right_server::start() {
    log_info(TAG, "START");
    while (true) {

        //Receive program:
        char program[BUFFER_SIZE] = {0};
        while (strlen(program) == 0) {
            read(this->left_server_input_channel, program, BUFFER_SIZE);
        }
        log_info_string(TAG, "PROGRAM!", program);

        //Send response to the middle server:
        write(this->left_server_output_channel, ACCEPT_STATUS, 7);

        //Save file:
        ofstream file;
        file.open(PATH);
        file << program;
        file.close();

        //Compile file:
        system("gcc  ../io/right_server/sample1.c -o ../io/right_server/sample1");
        system("rm ..io/right_server/sample1.c");

        //Send program to the middle server:
        write(this->output_middle_anonymous_gate, program, BUFFER_SIZE);
        char middle_response[BUFFER_SIZE] = {0};
        while (strlen(middle_response) == 0) {
            read(this->input_middle_anonymous_gate, middle_response, BUFFER_SIZE);
            log_info_string(TAG, "MIDDLE RESPONSE:", middle_response);
        }
        if (strcmp(middle_response, ACCEPT_STATUS) == 0) {
            log_info(TAG, "Program was sent!");
        }
        bzero(middle_response, BUFFER_SIZE);
        bzero(program, BUFFER_SIZE);
    }
}