#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
#include <util/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_builder/str_builder.h>

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
    const unsigned size_1 = name_list_1->get_size();
    const unsigned size_2 = name_list_2->get_size();
    if (size_1 != size_2) {
        return false;
    }
    bool is_equals = true;
    str_builder *builder = new str_builder();
    builder->append(*list_1);
    for (unsigned i = 0; i < size_2; ++i) {
        const str_hook *name = name_list_2->get(i);
        bool is_contains = false;
        for (unsigned j = 0; j < size_1; ++j) {
            const str_hook *element = name_list_1->get(j);
            if (name->equals_string(element)) {
                is_contains = true;
            }
        }
        if (!is_contains) {
            const char *name_str = name->to_string();
            is_equals = false;
            builder->append(' ');
            builder->append(name_str);
            delete name_str;
        }
    }
    *list_1 = builder->pop();
    return is_equals;
}