#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <cstring>
#include <cstdio>

namespace lib4aio {
    static str_hook *border_trim(
            const str_hook *hook,
            const bool has_left,
            const bool has_right,
            bool (*trim_condition)(const char)
    )
    {
        auto src = hook->get_string();
        auto new_hook = new str_hook(hook);
        auto src_length = hook->get_size();
        if (src_length == 0) {
            return new_hook;
        }
        if (has_left) {
            while (trim_condition(src[new_hook->start])) {
                new_hook->start++;
            }
        }
        if (has_right) {
            while (trim_condition(src[new_hook->end - 1])) {
                new_hook->end--;
            }
        }
        if (new_hook->start > new_hook->end) {
            new_hook->start = hook->start;
            new_hook->end = hook->start;
        }
        return new_hook;
    }

    str_hook *str_hook::trim_by_start() const
    {
        return border_trim(this, true, false, is_space);
    }

    str_hook *str_hook::trim_by_end() const
    {
        return border_trim(this, false, true, is_space);
    }

    str_hook *str_hook::trim() const
    {
        return border_trim(this, false, true, is_space);
    }

    str_hook *str_hook::trim_with_line_break_by_start() const
    {
        return border_trim(this, true, false, is_space_or_line_break);
    }

    str_hook *str_hook::trim_with_line_break_by_end() const
    {
        return border_trim(this, false, true, is_space_or_line_break);
    }

    str_hook *str_hook::trim_with_line_break() const
    {
        return border_trim(this, true, false, is_space_or_line_break);
    }

    bool str_hook::ends_with(const char *string) const
    {
        const unsigned str_length = (unsigned) strlen(string);
        if (str_length < this->get_size()) {
            unsigned counter = 0;
            for (unsigned j = this->end - str_length; j < this->end; ++j) {
                if (this->string_ptr[j] != string[counter++]) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }
}