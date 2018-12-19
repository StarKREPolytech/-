#include <string.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>

namespace lib4aio {

    static auto EMPTY_STRING = "";

    static bool is_correct_src_prefix(
            const unsigned src_length,
            const unsigned prefix_length,
            const unsigned suffix_length,
            const char *src,
            char **dst
    )
    {
        if (src_length == 0) {
            *dst = (char *) new_object_array(2, sizeof(char));
            return false;
        }
        if (prefix_length + suffix_length > src_length || (prefix_length == 0 && suffix_length == 0)) {
            *dst = static_cast<char *>(new_object_array(src_length + 1, sizeof(char)));
            strcpy(*dst, src);
            return false;
        }
        return true;
    }

    char *remove_prefix_suffix(const char *src, const char *prefix, const char *suffix)
    {
        unsigned src_length = (unsigned) strlen(src);
        unsigned prefix_length = (unsigned) strlen(prefix);
        unsigned suffix_length = (unsigned) strlen(suffix);
        char *dst;
        if (!is_correct_src_prefix(src_length, prefix_length, suffix_length, src, &dst)) {
            return dst;
        }
        auto left_part = 0;
        for (auto j = 0; j < prefix_length; ++j) {
            if (src[j] != prefix[j]) {
                left_part = 0;
                break;
            } else {
                left_part++;
            }
        }
        auto right_part = 0;
        for (auto j = 0; j < suffix_length; ++j) {
            if (src[src_length - suffix_length + j] != suffix[j]) {
                right_part = 0;
                break;
            } else {
                right_part++;
            }
        }
        auto num_of_elements = src_length - left_part - right_part;
        dst = (char *) new_object_array(num_of_elements + 1, sizeof(char));
        for (auto i = left_part; i < src_length - right_part; ++i) {
            dst[i - left_part] = src[i];
        }
        return dst;
    }

    char *remove_prefix(const char *src, const char *prefix)
    {
        return remove_prefix_suffix(src, prefix, EMPTY_STRING);
    }

    char *remove_suffix(const char *src, const char *suffix)
    {
        return remove_prefix_suffix(src, EMPTY_STRING, suffix);
    }

    bool starts_with_prefix(const char *src, const char *prefix)
    {
        auto size = strlen(prefix);
        for (auto i = 0; i < size; ++i) {
            if (src[i] != prefix[i]) {
                return false;
            }
        }
        return true;
    }

    bool ends_with_suffix(const char *src, const char *suffix)
    {
        auto src_length = strlen(src);
        auto suffix_length = strlen(suffix);
        for (auto j = 0; j < suffix_length; ++j) {
            if (src[src_length - suffix_length + j] != suffix[j]) {
                return false;
            }
        }
        return true;
    }
}