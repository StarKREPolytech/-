#include <cstdio>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

namespace lib4aio
{
    void log_info_str_hook(const char *tag, const char *message, const str_hook *hook)
    {
        auto end = hook->end;
        auto source_string = hook->get_string();
        printf("\n%s: %s -", tag, message);
        for (int i = hook->start; i < end; ++i) {
            printf("%c", source_string[i]);
        }
        printf("-\n");
    }

    std::ostream &operator<<(std::ostream &os, const str_hook *hook)
    {
        auto end = hook->end;
        auto source_string = hook->get_string();
        os << " ";
        for (int i = hook->start; i < end; ++i) {
            os << source_string[i];
        }
        os << " ";
        return os;
    }
}