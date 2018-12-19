#include <cstring>
#include <malloc.h>
#include <lib4aio_cpp_headers/utils/point_watcher/point_watcher.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio_cpp_sources/utils/array_list_utils/array_list.cpp>
#include <lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>

namespace lib4aio
{

    static void create_left_part(
            const str_hook *hook,
            const int left_index,
            unsigned delimiter_length,
            lib4aio::str_hook_list *list
    )
    {
        auto left_hook = new str_hook(hook);
        left_hook->start = hook->start;
        left_hook->end = left_index - delimiter_length;
        list->add(left_hook);
    }

    static void create_right_part(
            const str_hook *hook,
            const unsigned right_index,
            str_hook_list *list
    )
    {
        auto right_hook = new str_hook(hook);
        right_hook->start = right_index;
        right_hook->end = hook->end;
        list->add(right_hook);
    }

    static str_hook_list *create_str_hook_list_without_split(const str_hook *hook)
    {
        auto list = new str_hook_list();
        auto new_hook = new str_hook(hook);
        list->add(new_hook);
        return list;
    }

    str_hook_list *str_hook::split_by_comma() const
    {
        return this->split_by_char_condition(is_comma);
    }

    str_hook_list *str_hook::split_by_space() const
    {
        return this->split_by_char_condition(is_space);
    }

    str_hook_list *str_hook::split_by_line_break() const
    {
        return this->split_by_char_condition(is_line_break);
    }

    str_hook_list *str_hook::split_by_whitespace() const
    {
        return this->split_by_char_condition(is_space_or_line_break);
    }

    str_hook_list *str_hook::split(const char *delimiter) const
    {
        auto src = this->string_ptr;
        auto src_length = this->get_size();
        auto delimiter_length = strlen(delimiter);
        //Is empty string:
        if (src_length < 1 || delimiter_length == 0) {
            return create_str_hook_list_without_split(this);
        }
        //Create split indices:
        unsigned *indices = static_cast<unsigned *>(new_object_array(src_length, sizeof(int)));
        unsigned pointers = 0;
        //Mark split points:
        for (unsigned i = this->start; i < this->end; ++i) {
            int result = 0;
            while (result == 0) {
                for (unsigned j = 0; j < delimiter_length; ++j) {
                    if (src[i + j] != delimiter[j]) {
                        result = -1;
                        break;
                    }
                }
                if (result == 0) {
                    i += delimiter_length;
                    indices[pointers] = i;
                    pointers++;
                }
            }
        }
        //Cannot find points:
        if (pointers == 0) {
            //----------------------------------------------------------------------------------------------------------
            //찌꺼기 수집기:
            free(indices);
            //----------------------------------------------------------------------------------------------------------
            return create_str_hook_list_without_split(this);
        }
        reallocate_object_array(indices, pointers, sizeof(unsigned));
        auto list = new str_hook_list();
        //Create left part:
        create_left_part(this, indices[0], delimiter_length, list);
        //Create middle parts:
        if (pointers > 1) {
            //From second delimiter:
            for (int j = 0; j < pointers - 1; ++j) {
                auto new_hook = new str_hook();
                auto start_position = indices[j];
                auto hold_length = indices[j + 1] - indices[j] - delimiter_length;
                if (hold_length > 0) {
                    new_hook->start = start_position;
                    new_hook->end = start_position + hold_length;
                }
                new_hook->string_ptr = src;
                list->add(new_hook);
            }
        }
        //Create right part:
        create_right_part(this, indices[pointers - 1], list);
        //--------------------------------------------------------------------------------------------------------------
        //찌꺼기 수집기:
        free(indices);
        //--------------------------------------------------------------------------------------------------------------
        return list;
    }

    str_hook_list *str_hook::split_by_char_condition(bool (*char_condition)(const char)) const
    {
        auto src = this->string_ptr;
        auto start = this->start;
        auto end = this->end;
        auto hooks = new str_hook_list();
        auto watcher = new point_watcher();
        watcher->start = start;
        for (unsigned i = start; i < end; ++i) {
            auto symbol = src[i];
            auto is_split_point = char_condition(symbol);
            if (is_split_point) {
                auto new_hook = new str_hook(src);
                new_hook->start = watcher->start;
                new_hook->end = i;
                hooks->add(new_hook);
                watcher->start = i + 1;
            }
        }
        auto last_hook = new str_hook(src);
        last_hook->start = watcher->start;
        last_hook->end = end;
        hooks->add(last_hook);
        //--------------------------------------------------------------------------------------------------------------
        //찌꺼기 수집기:
        delete watcher;
        //--------------------------------------------------------------------------------------------------------------
        return hooks;
    }
}