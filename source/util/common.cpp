#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
#include <util/common.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>
#include <lib4aio/lib4aio_cpp_headers/utils/str_hook_utils/str_hook_list/str_hook_list.h>

int *create_anonymous_pipeline() {
    int *file_descriptor_pipeline = static_cast<int *>(calloc(2, sizeof(int)));
    pipe(file_descriptor_pipeline);
    return file_descriptor_pipeline;
}

bool is_equals_lists(const char *list_1, const char *list_2) {
    const unsigned size_1 = list_1->get_size();
    const unsigned size_2 = list_2->get_size();
    if (size_1 != size_2) {
        return false;
    }
    for (unsigned i = 0; i < size_1; ++i) {
        const str_hook *hook = list_1->get(i);
        const bool is_contains = list_2->contains_by(
                [&hook](const str_hook *element) -> bool {
                    return hook->equals_string(element);
                });
        if (!is_contains) {
            return false;
        }
    }
    return true;
}