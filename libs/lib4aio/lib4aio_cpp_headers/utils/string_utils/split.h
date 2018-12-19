#include <lib4aio_cpp_headers/utils/struct_list/struct_list.h>

namespace lib4aio {

#ifndef LIB4AIO_STRING_UTILS_SPLIT_H
#define LIB4AIO_STRING_UTILS_SPLIT_H

    struct_list *split_by_string(const char *src, const char *delimiter);

    struct_list *split_by_space(const char *src);

    struct_list *split_by_comma(const char *src);

    struct_list *split_by_line_break(const char *src);

#endif //LIB4AIO_STRING_UTILS_SPLIT_H

}