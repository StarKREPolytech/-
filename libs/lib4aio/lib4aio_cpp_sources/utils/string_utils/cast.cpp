#include <cstring>
#include <cstdlib>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <lib4aio_cpp_headers/utils/error_utils/error_utils.h>

namespace lib4aio {

    const static char CHAR_SHIFT = '0';

    const static int DIGIT_SHIFT = 10;

    const static int LEFT_BORDER = 0;

    const static char DOT = '.';

    const static int NANO = 9;

    char *int_to_string(const int src)
    {
        auto division = src;
        unsigned int_size_in_string = 0;
        while (division != 0) {
            division = division / 10;
            int_size_in_string = int_size_in_string + 1;
        }
        char *integer_array;
        auto negative_shift = 0;
        if (int_size_in_string > 0) {
            if (src < 0) {
                negative_shift = 1;
            }
            integer_array = (char *) new_object_array(int_size_in_string + 1 + negative_shift, sizeof(char));
            division = src;
            integer_array[0] = '-';
            auto pointer = int_size_in_string - 1 + negative_shift;
            while (division != 0) {
                (integer_array)[pointer] = (char) (abs(division % 10) + '0');
                division = division / 10;
                pointer--;
            }
        } else {
            integer_array = (char *) new_object_array(2, sizeof(char));
            integer_array[0] = '0';
            int_size_in_string = 1;
        }
        char *dst = (char *) new_object_array(int_size_in_string + 1 + negative_shift, sizeof(char));
        for (unsigned k = 0; k < int_size_in_string + negative_shift; ++k) {
            dst[k] = integer_array[k];
        }
        free(integer_array);
        return dst;
    }

    char *double_to_string(const double src)
    {
        auto sb = new str_builder();
        //Put int part:
        auto int_part = (int) src;
        auto int_string = int_to_string(int_part);
        sb->append(int_string);
        //Put dot:
        sb->append(DOT);
        //Put fractional part:
        auto fractional_part = src - int_part;
        for (auto i = 0; i < NANO; ++i) {
            fractional_part *= DIGIT_SHIFT;
            auto digit = (int) fractional_part;
            auto symbol = (const char) (digit + '0');
            sb->append(symbol);
            fractional_part -= digit;
        }
        //Extract string builder accumulator:
        auto right_border = sb->size() - 1;
        auto sb_acc = sb->get_string();
        //Shift empty cells:
        while (right_border >= 0) {
            auto symbol = sb_acc[right_border];
            if (symbol != '0') {
                break;
            } else {
                right_border--;
            }
        }
        auto result = substring(sb_acc, LEFT_BORDER, right_border + 1);
        //--------------------------------------------------------------------------------------------------------------
        //찌꺼기 수집기:
        delete sb;
        //--------------------------------------------------------------------------------------------------------------
        return result;
    }

    int string_to_int(const char *string)
    {
        auto length = strlen(string);
        auto start = 0;
        auto is_negative = is_minus_sign(string[0]);
        if (is_negative) {
            start++;
        }
        auto result = 0;
        for (auto i = start; i < length; i++) {
            auto digit = string[i] - '0';
            result = result * 10 + digit;
        }
        if (is_negative) {
            result *= -1;
        }
        return result;
    }

    double string_to_double(const char *string)
    {
        auto length = strlen(string);
        auto i = 0;
        auto is_negative = is_minus_sign(string[i]);
        if (is_negative) {
            i++;
        }
        auto integer_part = 0;
        while (!is_dot(string[i])) {
            integer_part = integer_part * DIGIT_SHIFT + (string[i++] - CHAR_SHIFT);
        }
        auto fraction_part = 0.0;
        auto fraction_counter = DIGIT_SHIFT;
        for (auto j = i + 1; j < length; ++j) {
            auto digit = ((double) (string[j] - CHAR_SHIFT) / fraction_counter);
            fraction_part += digit;
            fraction_counter *= DIGIT_SHIFT;
        }
        auto result = ((double) integer_part) + fraction_part;
        if (is_negative) {
            result *= -1;
        }
        return result;
    }
}