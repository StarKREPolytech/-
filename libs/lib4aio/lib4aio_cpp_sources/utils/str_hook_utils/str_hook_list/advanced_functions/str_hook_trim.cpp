#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>

namespace lib4aio
{
    str_hook_list *str_hook_list::trim() const
    {
        auto new_list = new str_hook_list();
        for (unsigned i = 0; i < this->get_size(); ++i) {
            auto new_hook = this->get(i)->trim();
            new_list->add(new_hook);
        }
        return new_list;
    }

    str_hook_list *str_hook_list::trim_with_line_break() const
    {
        auto new_list = new str_hook_list();
        for (unsigned i = 0; i < this->get_size(); ++i) {
            auto new_hook = this->get(i)->trim();
            new_list->add(new_hook);
        }
        return new_list;
    }
}