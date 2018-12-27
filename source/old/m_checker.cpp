#include <cstring>
#include <zconf.h>
#include <util/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>
#include <string>
#include <signal.h>

using namespace lib4aio;

using namespace std;

#define TAG "M_CHECKER"

#define BUFFER_SIZE 1024

#define ACCEPT_STATUS "ACCEPT"

static const char *const PATH = "../io/m_server/sample1.txt";

int check_last_modified_time(
        const int input_gate,
        const int output_gate,
        const char *request
)
{
    log_info(TAG, "STARTED CHECKER!");
    string::size_type type;
    const long last_modified_file = get_file_last_modified_time(PATH);
    const long input_last_modified_time = stol(request, &type);
    int flag;
    if (last_modified_file < input_last_modified_time) {
        log_info_string(TAG, "SEND SYNC_REQUEST TO ARCHIVER", request);
        write(output_gate, request, BUFFER_SIZE);
        char response[BUFFER_SIZE] = {0};
        while (strlen(response) == 0) {
            read(input_gate, response, BUFFER_SIZE);
        }
        log_info_string(TAG, "ARCHIVER RESPONSE", response);
        if (strcmp(response, ACCEPT_STATUS) == 0) {
            flag = SIGUSR1;
            log_info(TAG, "YES!");
        } else {
            flag = SIGUSR2;
            log_info(TAG, "NO 1!");
        }
    } else {
        flag = SIGUSR2;
        log_info(TAG, "NO 2!");
    }
    return flag;
}