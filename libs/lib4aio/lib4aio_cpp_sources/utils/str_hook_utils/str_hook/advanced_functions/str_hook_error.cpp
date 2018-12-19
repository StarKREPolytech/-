#include <cstdio>
#include <cstdlib>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

#define ERROR_TAG "ERROR"

namespace lib4aio
{
    void throw_error_with_str_hook(const char *tag, const char *message, const str_hook *hook)
    {
        auto end = hook->end;
        auto source_string = hook->get_string();
        printf("\n%s %s: %s -", ERROR_TAG, tag, message);
        for (int i = hook->start; i < end; ++i) {
            printf("%c", source_string[i]);
        }
        printf("-\n");
        exit(1);
    }
}