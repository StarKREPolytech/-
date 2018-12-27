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

#define PATH "../io/left_server/sample1.c"

void o_left_server::start() {
    log_info(TAG, "START");
    while (true) {

        //Receive program:
        char program[BUFFER_SIZE] = {0};
        while (strlen(program) == 0) {
            read(this->middle_server_input_channel, program, BUFFER_SIZE);
        }
        log_info_string(TAG, "PROGRAM!", program);

        //Send response to the middle server:
        write(this->middle_server_output_channel, ACCEPT_STATUS, 7);

        //Save file:
        ofstream file;
        file.open(PATH);
        file << program;
        file.close();

        //Compile file:
        system("gcc  ../io/left_server/sample1.c -o ../io/left_server/sample1");

        if (fork() == 0) {

            //Launch a program:
            system("../io/left_server/sample1");
            log_info(TAG, "PROGRAM WAS LAUNCHED!!!");
        } else {

            //Send program to the right server:
            write(this->output_right_anonymous_gate, program, BUFFER_SIZE);
            char right_response[BUFFER_SIZE] = {0};
            while (strlen(right_response) == 0) {
                read(this->input_right_anonymous_gate, right_response, BUFFER_SIZE);
                log_info_string(TAG, "RIGHT RESPONSE:", right_response);
            }
            if (strcmp(right_response, ACCEPT_STATUS) == 0) {
                log_info(TAG, "Program was sent!");
            }
            bzero(right_response, BUFFER_SIZE);
            bzero(program, BUFFER_SIZE);
        }

    }
}