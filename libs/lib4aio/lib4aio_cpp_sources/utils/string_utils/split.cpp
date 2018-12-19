#include <string.h>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/string_utils/split.h>
#include <lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio_cpp_headers/utils/int_list_utils/int_list.h>

//#define LIB4AIO_STRING_UTILS_SPLIT_DEBUG

#ifdef LIB4AIO_STRING_UTILS_SPLIT_DEBUG

#include <lib4aio_cpp_headers/utils/log_utils/log_utils.h>

#endif

#define LIB4AIO_STRING_UTILS_SPLIT_TAG "LIB4AIO_STRING_UTILS_SPLIT"

namespace lib4aio {

    static const char *COMMA = ",";

    static const char *SPACE = " ";

    static const char *LINE_BREAK = "\n";

    static void create_left_part(
            const char *src,
            const int_list *indices,
            unsigned delimiter_length,
            struct_list *strings
    )
    {
        const unsigned first_point = (unsigned) (indices->values[0]);
        char *left_string = substring(src, 0, first_point - delimiter_length);
#ifdef LIB4AIO_STRING_UTILS_SPLIT_DEBUG
        log_info_string(LIB4AIO_STRING_UTILS_SPLIT_TAG, "Left part:", left_string);
#endif
        add_struct_in_list(strings, left_string);
    }

    static void create_right_part(
            const char *src,
            const int_list *indices,
            struct_list *string_list,
            unsigned src_length
    )
    {
        const unsigned last_point = (unsigned) (indices->values[indices->size - 1]);
        char *right_string = substring(src, last_point, src_length);
#ifdef LIB4AIO_STRING_UTILS_SPLIT_DEBUG
        log_info_string(LIB4AIO_STRING_UTILS_SPLIT_TAG, "Right part:", right_string);
#endif
        add_struct_in_list(string_list, right_string);
    }

    struct_list *split_by_comma(const char *src)
    {
        return split_by_string(src, COMMA);
    }

    struct_list *split_by_space(const char *src)
    {
        return split_by_string(src, SPACE);
    }

    struct_list *split_by_line_break(const char *src)
    {
        return split_by_string(src, LINE_BREAK);
    }

    struct_list *split_by_string(const char *src, const char *delimiter)
    {
        if (!src) {
            return nullptr;
        }
#ifdef LIB4AIO_STRING_UTILS_SPLIT_DEBUG
        log_info_string(LIB4AIO_STRING_UTILS_SPLIT_TAG, "Input src:", src);
#endif
        struct_list *string_list = new_struct_list(sizeof(char *));
        const unsigned src_length = static_cast<const unsigned int>(strlen(src));
        //Is empty string:
        if (src_length < 1 || are_equal_strings(delimiter, "")) {
            add_struct_in_list(string_list, new_string(src));
            return string_list;
        }
        int_list *indices = new_int_list();
        const unsigned delimiter_length = static_cast<const unsigned int>(strlen(delimiter));
        //Mark split points:
        for (unsigned i = 0; i < src_length; ++i) {
            bool matches = true;
            while (matches) {
                for (unsigned j = 0; j < delimiter_length; ++j) {
                    if (src[i + j] != delimiter[j]) {
                        matches = false;
                        break;
                    }
                }
                if (matches) {
                    i += delimiter_length;
                    add_int_in_list(indices, i);
                }
            }
        }
        //Cannot find points:
        const unsigned split_point_count = indices->size;
        if (split_point_count == 0) {
            add_struct_in_list(string_list, new_string(src));
            //----------------------------------------------------------------------------------------------------------
            //찌꺼기 수집기:
            free_int_list(indices);
            //----------------------------------------------------------------------------------------------------------
            return string_list;
        }
        //Parts more than split_point_count by 1:
        //Create left part:
        create_left_part(src, indices, delimiter_length, string_list);
        //Create middle parts:
        if (split_point_count > 1) {
            //From second delimiter:
            for (unsigned j = 0; j < split_point_count - 1; ++j) {
                const unsigned start = (unsigned) indices->values[j];
                const unsigned end = indices->values[j + 1] - delimiter_length;
                char *string = nullptr;
                if (end - start > 0) {
                    string = substring(src, start, end);
                } else {
                    string = new_string("");
                }
#ifdef LIB4AIO_STRING_UTILS_SPLIT_DEBUG
                log_info_string(LIB4AIO_STRING_UTILS_SPLIT_TAG, "Chunk:", string);
#endif
                add_struct_in_list(string_list, string);
            }
        }
        //Create right part:
        create_right_part(src, indices, string_list, src_length);
        //--------------------------------------------------------------------------------------------------------------
        //찌꺼기 수집기:
        free_int_list(indices);
        //--------------------------------------------------------------------------------------------------------------
        return string_list;
    }
};