#include <cstdio>
#include <stdlib.h>

namespace lib4aio
{

    void *new_object(const unsigned size_of_object)
    {
        void *object = calloc(1, size_of_object);
        if (!object) {
            perror("Can'n allocate memory for object!");
            exit(1);
        }
        return object;
    }

    void *new_object_array(const unsigned number_of_elements, const unsigned size_of_object)
    {
        void *object_array = calloc(number_of_elements, size_of_object);
        if (!object_array) {
            perror("Can not allocate memory for object array!");
            exit(1);
        }
        return object_array;
    }

    void *reallocate_object_array(
            void *object_array,
            const unsigned new_number_of_elements,
            const unsigned size_of_object
    )
    {
        object_array = realloc(object_array, new_number_of_elements * sizeof(size_of_object));
        if (!object_array) {
            perror("Can not reallocate memory for object array!");
            exit(1);
        } else {
            return realloc(object_array, new_number_of_elements * size_of_object);
        }
    }

    void free_object(void *object)
    {
        if (object) {
            free(object);
        }
    }
}