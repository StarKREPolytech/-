namespace lib4aio
{

#include <stdio.h>
#include <lib4aio_cpp_headers/utils/log_utils/log_utils.h>

#define TRUE "true"

#define FALSE "false"

    void log_info(const char *tag, const char *message)
    {
        printf("\n%s: %s\n", tag, message);
    }

    void log_info_string(const char *tag, const char *message, const char *value)
    {
        printf("\n%s: %s -%s-\n", tag, message, value);
    }

    void log_info_char(const char *tag, const char *message, const char value)
    {
        printf("\n%s: %s -%c-\n", tag, message, value);
    }

    void log_info_double(const char *tag, const char *message, const double value)
    {
        printf("\n%s: %s -%lf-\n", tag, message, value);
    }

    void log_info_int(const char *tag, const char *message, const int value)
    {
        printf("\n%s: %s -%d-\n", tag, message, value);
    }

    void log_info_boolean(const char *tag, const char *message, const bool value)
    {
        const char *boolean_string = nullptr;
        if (value) {
            boolean_string = TRUE;
        } else {
            boolean_string = FALSE;
        }
        printf("\n%s: %s -%s-\n", tag, message, boolean_string);
    }
}