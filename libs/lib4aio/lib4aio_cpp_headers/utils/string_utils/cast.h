#ifndef LIB4AIO_STRING_UTILS_CAST_H
#define LIB4AIO_STRING_UTILS_CAST_H

namespace lib4aio {

    char *int_to_string(const int src);

    char *double_to_string(const double src);

    char *boolean_to_string(const bool src);

    int string_to_int(const char *string);

    double string_to_double(const char *string);

    bool string_to_boolean(const char *string);

}

#endif //LIB4AIO_STRING_UTILS_CAST_H