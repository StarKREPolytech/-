#include <stdlib.h>
#include <lib4aio_cpp_headers/utils/struct_list/struct_list.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>

namespace lib4aio
{

#define INIT_CAPACITY 2

    struct_list *new_struct_list(const unsigned type)
    {
        struct_list *list = static_cast<struct_list *>(new_object(sizeof(struct_list)));
        list->type = type;
        list->capacity = INIT_CAPACITY;
        list->size = 0;
        list->elements = static_cast<void **>(new_object_array(INIT_CAPACITY, sizeof(type)));
        return list;
    }

    void update_memory_in_struct_list(struct_list *list)
    {
        if (list->size == list->capacity) {
            list->capacity = list->capacity * 2;
            list->elements = static_cast<void **>(reallocate_object_array(
                    list->elements,
                    list->capacity,
                    list->type
            ));
        }
    }

    void add_struct_in_list(struct_list *list, void *element)
    {
        update_memory_in_struct_list(list);
        list->elements[list->size++] = element;
    }

    void free_structs_in_list(struct_list *list, std::function<void(void *)> func)
    {
        void **elements = list->elements;
        for (unsigned i = 0; i < list->size; ++i) {
            void *old_string = elements[i];
            list->elements[i] = nullptr;
            func(old_string);
        }
    }


    void free_struct_list(struct_list *list)
    {
        const unsigned size = list->size;
        for (unsigned i = 0; i < size; ++i) {
            list->elements[i] = nullptr;
        }
        free(list->elements);
        free(list);
    }
}