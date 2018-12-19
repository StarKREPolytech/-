#include <functional>

namespace lib4aio
{
#ifndef LIB4AIO_STRUCT_LIST_H
#define LIB4AIO_STRUCT_LIST_H

    typedef struct struct_list
    {
        unsigned type;
        unsigned capacity;
        unsigned size;
        void **elements;
    } struct_list;

    struct struct_list *new_struct_list(const unsigned type);

    void add_struct_in_list(struct_list *list, void *element);

    void foreach_in_struct_list(struct_list *list, void (*action)(void *));

    void free_structs_in_list(struct_list *list, std::function<void(void *)> func);

    void free_struct_list(struct_list *list);

#endif //LIB4AIO_STRUCT_LIST_H
}