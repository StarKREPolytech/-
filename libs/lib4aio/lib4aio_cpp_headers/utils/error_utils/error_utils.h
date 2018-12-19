namespace lib4aio {
#ifndef LIB4AIO_ERROR_UTILS_H
#define LIB4AIO_ERROR_UTILS_H

    void throw_error(
            const char *message
    ) __attribute__ ((noreturn));

    void throw_error_with_tag(
            const char *tag,
            const char *message
    ) __attribute__ ((noreturn));

    void throw_error_with_details(
            const char *tag,
            const char *message,
            const char *value
    ) __attribute__ ((noreturn));

#endif //LIB4AIO_ERROR_UTILS_H

}