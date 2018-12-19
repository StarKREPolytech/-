#include <string.h>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>

namespace lib4aio {

    static char *abstract_border_trim(
            const char *src,
            const bool has_left,
            const bool has_right,
            bool (*trim_condition)(const char)
    )
    {
        char *dst;
        auto src_length = strlen(src);
        if (src_length == 0) {
            dst = (char *) new_object_array(2, sizeof(char));
            return dst;
        }
        auto left_border = 0;
        auto right_border = 0;
        if (has_left) {
            while (trim_condition(src[left_border])) {
                left_border++;
            }
        }
        if (has_right) {
            while (trim_condition(src[src_length - right_border - 1])) {
                right_border++;
            }
        }
        unsigned number_of_elements;
        if (left_border < src_length - 1) {
            number_of_elements = src_length - left_border - right_border + 1;
        } else {
            number_of_elements = 2;
        }
        dst = static_cast<char *>(new_object_array(number_of_elements + 1, sizeof(char)));
        for (auto i = left_border; i < src_length - right_border; ++i) {
            dst[i - left_border] = src[i];
        }
        dst[src_length - left_border] = '\0';
        return dst;
    }

    char *trim_start(const char *src)
    {
        return abstract_border_trim(src, true, false, is_space);
    }

    char *trim_end(const char *src)
    {
        return abstract_border_trim(src, false, true, is_space);
    }

    char *trim(const char *src)
    {
        return abstract_border_trim(src, true, true, is_space);
    }

    char *trim_with_line_break(const char *src)
    {
        return abstract_border_trim(src, true, true, is_space_or_line_break);
    }

    char *trim_start_with_line_break(const char *src)
    {
        return abstract_border_trim(src, true, false, is_space_or_line_break);
    }

    char *trim_end_with_line_break(const char *src)
    {
        return abstract_border_trim(src, false, true, is_space_or_line_break);
    }

    char **trim_all(const char **strings, const unsigned number_of_strings)
    {
        auto dst = static_cast<char **>(new_object_array(
                number_of_strings,
                sizeof(char *))
        );
        for (int i = 0; i < number_of_strings; ++i) {
            dst[i] = trim(strings[i]);
        }
        return dst;
    }

    char **trim_all_with_line_break(const char **strings, const unsigned number_of_strings)
    {
        auto dst = static_cast<char **>(new_object_array(
                number_of_strings,
                sizeof(char *))
        );
        for (int i = 0; i < number_of_strings; ++i) {
            dst[i] = trim_with_line_break(strings[i]);
        }
        return dst;
    }
}