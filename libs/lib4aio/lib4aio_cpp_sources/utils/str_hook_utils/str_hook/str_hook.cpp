//native
#include <cctype>
#include <cstring>
//lib4aio:
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/point_watcher/point_watcher.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

namespace lib4aio {

    str_hook::str_hook()
    {
        this->string_ptr = nullptr;
        this->start = 0;
        this->end = 0;
    }

    str_hook::str_hook(const char *source_string, const unsigned start, const unsigned end)
    {
        this->string_ptr = source_string;
        this->start = start;
        this->end = end;
    }

    str_hook::str_hook(const char *source_string, const point_watcher *watcher)
    {
        this->string_ptr = source_string;
        this->start = watcher->start;
        this->end = watcher->end;
    }

    str_hook::str_hook(const str_hook *other)
    {
        this->string_ptr = other->string_ptr;
        this->start = other->start;
        this->end = other->end;
    }

    str_hook::str_hook(const char *source_string)
    {
        this->string_ptr = source_string;
        this->start = 0;
        this->end = (unsigned) strlen(source_string);
    }

    str_hook::~str_hook()
    {}

    unsigned str_hook::get_size() const
    {
        return this->end - this->start;
    }

    const char *str_hook::get_string() const
    {
        return this->string_ptr;
    }

    char str_hook::get_absolute_char(const unsigned index) const
    {
        return this->string_ptr[index];
    }

    char str_hook::get_relative_char(const unsigned index) const
    {
        return this->string_ptr[this->start + index];
    }

    bool str_hook::equals_string(const str_hook *other) const
    {
        auto size_1 = this->get_size();
        auto size_2 = other->get_size();
        if (size_1 != size_2) {
            return false;
        }
        auto str_1 = this->string_ptr;
        auto str_2 = other->string_ptr;
        auto start_1 = this->start;
        auto start_2 = other->start;
        for (int i = 0; i < size_1; ++i) {
            if (str_1[start_1 + i] != str_2[start_2 + i]) {
                return false;
            }
        }
        return true;
    }

    bool str_hook::is_word() const
    {
        auto length = this->get_size();
        if (length < 1) {
            return false;
        }
        auto first_symbol = this->get_relative_char(0);
        if (isalpha(first_symbol)) {
            if (length > 1) {
                for (unsigned i = 1; i < length; ++i) {
                    auto symbol = this->get_relative_char(i);
                    if (!isalnum(symbol)) {
                        return false;
                    }
                }
            }
            return true;
        } else {
            return false;
        }

    }

    bool str_hook::equals_string(const char *string) const
    {
        auto hook_size = this->get_size();
        auto str_length = strlen(string);
        if (hook_size != str_length) {
            return false;
        }
        auto hooked_string = this->string_ptr;
        auto start = this->start;
        for (int i = 0; i < hook_size; ++i) {
            if (hooked_string[start + i] != string[i]) {
                return false;
            }
        }
        return true;
    }

    bool str_hook::is_thin() const
    {
        for (unsigned i = this->start; i < this->end; ++i) {
            if (!is_space_or_line_break(this->string_ptr[i])) {
                return false;
            }
        }
        return true;
    }

    bool str_hook::is_empty() const
    {
        return this->get_size() <= 0;
    }

    bool str_hook::is_not_empty() const
    {
        return this->get_size() > 0;
    }

    char *str_hook::to_string() const
    {
        char *string = (char *) new_object_array(this->get_size() + 1, sizeof(char));
        unsigned position = 0;
        for (unsigned i = this->start; i < this->end; ++i) {
            string[position++] = this->string_ptr[i];
        }
        return string;
    }

    void str_hook::set_string(const char *string)
    {
        this->string_ptr = string;
    }

    bool str_hook::operator==(const str_hook &rhs) const
    {
        return this->equals_string(&rhs);
    }

    bool str_hook::operator!=(const str_hook &rhs) const
    {
        return !(rhs == *this);
    }

    bool str_hook::compare_hooks(const str_hook *o1, const str_hook *o2)
    {
        return *o1 == *o2;
    }
}