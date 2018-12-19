#include <lib4aio_cpp_headers/aio_explorer/aio_explorer.h>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

namespace lib4aio {

#define AIO_EXPLORER_INFO_TAG "AIO_EXPLORER_INFO"

#define AIO_EXPLORER_ERROR_TAG "AIO_EXPLORER_ERROR"

    str_hook *aio_explorer::explore_hook_scope(
            const unsigned start,
            const char left_border,
            const char right_border,
            const str_hook *parent_hook_ptr
    )
    {
        const char *str_ptr = parent_hook_ptr->get_string();
        str_hook *hook_scope = new str_hook(str_ptr);
        const unsigned end = parent_hook_ptr->end;
        unsigned i = start;
        bool in_string_expr_scope = false;
        bool in_content_scope = false;
        unsigned counter = 0;
        while (true) {
            const char c = str_ptr[i];
            if (!in_string_expr_scope) {
                if (c == left_border) {
                    if (!in_content_scope) {
                        in_content_scope = true;
                        hook_scope->start = i + 1;
                    }
                    counter++;
                }
                if (in_content_scope && c == right_border) {
                    counter--;
                    if (counter == 0) {
                        hook_scope->end = i;
                        break;
                    }
                }
            }
            if (is_single_quote(c)) {
                in_string_expr_scope = !in_string_expr_scope;
            }
            if (i == end) {
                throw_error_with_str_hook(AIO_EXPLORER_ERROR_TAG, "Invalid hook content!", parent_hook_ptr);
            }
            i++;
        }
        if (hook_scope->get_size() >= 0) {
            return hook_scope;
        } else {
            throw_error_with_str_hook(AIO_EXPLORER_ERROR_TAG, "Invalid hook content!", parent_hook_ptr);
        }
    }
}