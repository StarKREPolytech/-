#ifndef LIB4AIO_LOG_UTILS_H
#define LIB4AIO_LOG_UTILS_H

namespace lib4aio
{

    void log_info(const char *tag, const char *message);

    void log_info_string(const char *tag, const char *message, const char *value);

    void log_info_char(const char *tag, const char *message, const char value);

    void log_info_double(const char *tag, const char *message, const double value);

    void log_info_int(const char *tag, const char *message, const int value);

    void log_info_boolean(const char *tag, const char *message, const bool value);

}

#endif //LIB4AIO_LOG_UTILS_H