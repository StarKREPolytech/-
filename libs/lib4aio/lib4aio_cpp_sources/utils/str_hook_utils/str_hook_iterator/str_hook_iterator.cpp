#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook_iterator/str_hook_iterator.h>
#include <lib4aio_cpp_sources/utils/array_list_utils/array_list.cpp>

namespace lib4aio
{
    str_hook_iterator::str_hook_iterator()
    {
        this->hook_position = 0;
    }

    str_hook_iterator::str_hook_iterator(const str_hook_list *list)
    {
        this->hook_position = 0;
        this->hook_list = list;
    }

    str_hook_iterator::str_hook_iterator(
            const str_hook_list *list,
            unsigned const current_hook_index
    )
    {
        this->hook_position = 0;
        this->hook_list = list;
        this->current_hook_index = current_hook_index;
    }

    str_hook_iterator::str_hook_iterator(
            const str_hook_list *list,
            const unsigned current_hook_index,
            const unsigned current_position
    )
    {
        this->hook_position = 0;
        this->hook_list = list;
        this->current_hook_index = current_hook_index;
        this->hook_position = current_position;
    }

    bool str_hook_iterator::next()
    {
        //Extract hook_list:
        auto hook_list = this->hook_list;
        //Get current hook:
        auto current_hook_index = this->current_hook_index;
        auto current_hook = hook_list->get(current_hook_index);
        //Check next iterator_position:
        if (this->hook_position < current_hook->end - 1) {
            this->hook_position++;
            return true;
        } else {
            if (current_hook_index < hook_list->get_size() - 1) {
                auto next_hook = hook_list->get(++this->current_hook_index);
                this->hook_position = next_hook->start;
                return true;
            } else {
                this->hook_position++;
                return false;
            }
        }
    }

    str_hook_iterator::~str_hook_iterator()
    {}
}