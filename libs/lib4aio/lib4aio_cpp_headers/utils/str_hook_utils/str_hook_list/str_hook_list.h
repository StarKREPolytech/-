#ifndef LIB4AIO_STR_HOOK_LIST_H
#define LIB4AIO_STR_HOOK_LIST_H

#include <lib4aio_cpp_headers/utils/array_list_utils/array_list.h>

namespace lib4aio {

    class str_hook;

    class str_hook_list : public array_list<str_hook> {

    public:

        str_hook_list();

        str_hook_list *trim() const;

        str_hook_list *trim_with_line_break() const;
    };

    static void log_info(const char *const tag, const char *message, const str_hook_list *list);

    /**
     * Typedef utils.
     */

    typedef str_hook_list str_hook_chain;

}

#endif //LIB4AIO_STR_HOOK_LIST_H