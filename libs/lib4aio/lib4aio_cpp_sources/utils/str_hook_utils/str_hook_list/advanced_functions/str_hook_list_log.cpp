#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>

namespace lib4aio
{

    void log_info(const char *tag, const char *message, const str_hook_list *list)
    {
        auto size = list->get_size();
        for (int i = 0; i < size; ++i) {
            auto hook = list->get(i);
            log_info_str_hook(tag, message, hook);
        }
    }
}