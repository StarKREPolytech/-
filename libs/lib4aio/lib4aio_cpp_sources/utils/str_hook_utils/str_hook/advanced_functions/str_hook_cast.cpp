#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

#define CHAR_SHIFT '0'

#define DIGIT_SHIFT 10

namespace lib4aio
{
    static const char *STRING_HOOK_TAG = "STRING_HOOK";

    int str_hook::to_int() const
    {
        auto result = 0;
        auto string = this->string_ptr;
        auto end = this->end;
        for (int i = this->start; i < end; i++) {
            result = result * DIGIT_SHIFT + (string[i] - CHAR_SHIFT);
        }
        return result;
    }

    double str_hook::to_double() const
    {
        auto string = this->string_ptr;
        auto integer_part = 0;
        auto i = this->start;
        while (!is_dot(string[i])) {
            integer_part = integer_part * DIGIT_SHIFT + (string[i++] - CHAR_SHIFT);
        }
        auto fraction_part = 0.0;
        auto fraction_counter = DIGIT_SHIFT;
        auto end = this->end;
        for (int j = i + 1; j < end; ++j) {
            const double digit = ((double) (string[j] - CHAR_SHIFT) / fraction_counter);
            fraction_part += digit;
            fraction_counter *= DIGIT_SHIFT;
        }
        return ((double) integer_part) + fraction_part;
    }

    str_hook *str_hook::lower_quotes() const
    {
        return new str_hook(this->string_ptr, this->start + 1, this->end - 1);
    }
}