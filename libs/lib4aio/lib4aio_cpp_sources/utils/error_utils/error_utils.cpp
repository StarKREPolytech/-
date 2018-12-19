#include <stdio.h>
#include <stdlib.h>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>

namespace lib4aio {

    void throw_error(const char *message)
    {
        perror(message);
        exit(1);
    }

    void throw_error_with_tag(const char *tag, const char *message)
    {
        printf("\n%s: %s\n", tag, message);
        exit(1);
    }

    void throw_error_with_details(const char *tag, const char *message, const char *value)
    {
        printf("\n%s: %s %s\n", tag, message, value);
        exit(1);
    }
}