#include <ctype.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>

namespace lib4aio
{
    static const char *STRING_HOOK_TAG = "STRING_HOOK";

    bool str_hook::matches_int() const
    {
        auto string = this->string_ptr;
        auto length = this->get_size();
        auto start_position = this->start;
        if (length <= 0) {
            throw_error_with_str_hook(STRING_HOOK_TAG, "Empty string doesn't matches int!", this);
        }
        if (string[start_position] == '-') {
            if (length == 1) {
                throw_error_with_str_hook(STRING_HOOK_TAG, "Minus doesn't matches int!", this);
            }
            start_position++;
        }
        auto end = this->end;
        for (unsigned i = start_position; i < end; ++i) {
            if (!isdigit(string[i])) {
                return false;
            }
        }
        return true;
    }

    bool str_hook::matches_double() const
    {
        {
            auto string = this->string_ptr;
            auto length = this->get_size();
            auto was_dot = false;
            auto was_fraction = false;
            auto start_position = this->start;
            if (length <= 0) {
                throw_error_with_str_hook(STRING_HOOK_TAG, "Empty string doesn't matches double!", this);
            }
            if (is_minus_sign(string[start_position])) {
                if (length == 1) {
                    throw_error_with_str_hook(STRING_HOOK_TAG, "Minus doesn't matches double!", this);
                }
                start_position++;
            }
            auto end = this->end;
            for (unsigned i = start_position; i < end; ++i) {
                auto is_dot_cond = is_dot(string[i]);
                auto is_digit_cond = isdigit(string[i]);
                if (!is_digit_cond) {
                    if (is_dot_cond && !was_dot) {
                        was_dot = true;
                    } else {
                        return false;
                    }
                }
                if (is_digit_cond && was_dot) {
                    was_fraction = true;
                }
            }
            return was_dot && was_fraction;
        }
    }

    bool str_hook::matches_string() const
    {
        auto length = this->get_size();
        auto hooked_string = this->string_ptr;
        return length > 1
               && is_single_quote(hooked_string[this->start])
               && is_single_quote(hooked_string[this->end - 1]);
    }
}