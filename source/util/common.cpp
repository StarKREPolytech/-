#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
#include <util/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio/lib4aio_cpp_headers/utils/log_utils/log_utils.h>

int *create_anonymous_pipeline()
{
    int *file_descriptor_pipeline = static_cast<int *>(calloc(2, sizeof(int)));
    pipe(file_descriptor_pipeline);
    return file_descriptor_pipeline;
}

bool add_to_list(char **list_1, const char *list_2)
{
    str_hook *hook_1 = new str_hook(*list_1);
    str_hook *hook_2 = new str_hook(list_2);
    str_hook_list *name_list_1 = hook_1->split_by_whitespace();
    str_hook_list *name_list_2 = hook_2->split_by_whitespace();
    for (unsigned k = 0; k < name_list_1->get_size(); ++k) {
        log_info_str_hook("COMMON", "NAME 1:", name_list_1->get(k));
    }
    for (unsigned k = 0; k < name_list_2->get_size(); ++k) {
        log_info_str_hook("COMMON", "NAME 2:", name_list_2->get(k));
    }
    const unsigned size_1 = name_list_1->get_size();
    const unsigned size_2 = name_list_2->get_size();
    bool has_changed = false;
    str_builder *builder = new str_builder();
    builder->append(*list_1);
    for (unsigned i = 0; i < size_2; ++i) {
        const str_hook *name_2 = name_list_2->get(i);
        bool is_contains = false;
        for (unsigned j = 0; j < size_1; ++j) {
            const str_hook *name_1 = name_list_1->get(j);
            if (name_2->equals_string(name_1)) {
                is_contains = true;
            }
        }
        if (!is_contains) {
            const char *name_str = name_2->to_string();
            has_changed = true;
            builder->append(' ');
            builder->append(name_str);
            delete name_str;
        }
    }
    *list_1 = builder->pop();
    log_info_string("COMMON", "NEW LIST", *list_1);
    return has_changed;
}