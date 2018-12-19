#include <ctype.h>
#include <string.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>

namespace lib4aio
{

    bool matches_int(const char *string)
    {
        auto length = strlen(string);
        auto start = 0;
        if (length == 0) {
            return false;
        }
        auto first_symbol = string[0];
        if (is_minus_sign(first_symbol)) {
            if (length == 1) {
                return false;
            }
            start = 1;
        }
        for (int i = start; i < length; ++i) {
            auto symbol = string[i];
            if (!isdigit(symbol)) {
                return false;
            }
        }
        return true;
    }

    bool matches_double(const char *src)
    {
        auto was_dot = false;
        auto was_fraction = false;
        auto start = 0;
        auto length = strlen(src);
        if (length == 0) {
            return false;
        }
        if (is_minus_sign(src[start])) {
            if (length == 1) {
                return false;
            }
            start++;
        }
        if (!isdigit(src[start])) {
            return false;
        } else {
            start++;
        }
        for (int i = start; i < length; ++i) {
            auto symbol = src[i];
            auto is_digit = isdigit(symbol);
            if (!is_digit) {
                if (is_dot(src[i]) && !was_dot) {
                    was_dot = true;
                } else {
                    return false;
                }
            }
            if (is_digit && was_dot) {
                was_fraction = true;
            }
        }
        return was_dot && was_fraction;
    }

    bool matches_string(const char *src)
    {
        auto result = false;
        auto length = strlen(src);
        if (length > 1
            && src[0] == '\''
            && src[length - 1] == '\'') {
            result = true;
        }
        return result;
    }

    bool is_word(const char *line)
    {
        auto length = strlen(line);
        if (length < 1) {
            return false;
        }
        if (isalpha(line[0])) {
            if (length > 1) {
                for (auto i = 1; i < length; ++i) {
                    if (!isalnum(line[i])) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
}