#include <cstdlib>
#include <lib4aio_cpp_headers/utils/int_list_utils/int_list.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/log_utils/log_utils.h>

namespace lib4aio {

    int_list *new_int_list()
    {
        int_list *list = static_cast<int_list *>(new_object(sizeof(struct int_list)));
        list->capacity = 2;
        list->size = 0;
        list->values = static_cast<int *>(new_object_array(2, sizeof(int)));
        return list;
    }

    static void update_memory_in_int_list(int_list *list)
    {
        if (list->size == list->capacity) {
            list->capacity = list->capacity * 2;
            list->values = static_cast<int *>(reallocate_object_array(list->values, list->capacity, sizeof(int)));
            log_info_int("AAA", "SIZE:", list->capacity);
        }
    }

    void add_int_in_list(int_list *list, const int value)
    {
        update_memory_in_int_list(list);
        list->values[list->size] = value;
        list->size++;
    }

    void free_int_list(int_list *list)
    {
        free(list->values);
        free(list);
    }
}