namespace lib4aio
{
#ifndef LIB4AIO_STRING_UTILS_TRIM_H
#define LIB4AIO_STRING_UTILS_TRIM_H

    char *trim_start(const char *src);

    char *trim_end(const char *src);

    char *trim(const char *src);

    char **trim_all(const char **strings, const unsigned number_of_strings);

    char **trim_all_with_line_break(const char **strings, const unsigned number_of_strings);

    char *trim_start_with_line_break(const char *src);

    char *trim_end_with_line_break(const char *src);

    char *trim_with_line_break(const char *src);

#endif //LIB4AIO_STRING_UTILS_TRIM_H
}