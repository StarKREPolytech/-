#ifndef LIB4AIO_STRING_UTILS_SPLIT_H
#define LIB4AIO_STRING_UTILS_SPLIT_H

namespace lib4aio {

    bool matches_int(const char *string);

    bool matches_double(const char *src);

    bool matches_string(const char *src);

    bool matches_boolean(const char *string);

    bool is_word(const char *line);

}

#endif //LIB4AIO_STRING_UTILS_SPLIT_H