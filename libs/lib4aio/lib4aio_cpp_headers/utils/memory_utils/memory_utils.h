#ifndef LIB4AIO_MEMORY_UTILS_H
#define LIB4AIO_MEMORY_UTILS_H

namespace lib4aio {

    void *new_object(const unsigned size_of_object);

    void *new_object_array(const unsigned number_of_elements, const unsigned size_of_object);

    void *reallocate_object_array(
            void *object_array,
            const unsigned new_number_of_elements,
            const unsigned size_of_object
    );

    void free_object(void *object);
}

#endif //LIB4AIO_MEMORY_UTILS_H