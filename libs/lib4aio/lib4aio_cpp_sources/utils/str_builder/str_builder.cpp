
#include <string.h>
#include <stdlib.h>
#include <lib4aio_cpp_headers/utils/string_utils/common.h>
#include <lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>

namespace lib4aio {

#define INIT_CAPACITY 2

    str_builder::str_builder()
    {
        reset();
    }

    void str_builder::grow(const unsigned range)
    {
        const unsigned new_length = this->length + range + 1;
        if (new_length >= this->capacity) {
            while (new_length >= this->capacity) {
                this->capacity *= INIT_CAPACITY;
            }
            char *new_string = (char *) new_object_array(this->capacity, sizeof(char));
            strcpy(new_string, this->string_value);
            free(this->string_value);
            this->string_value = new_string;
        }
    }

    void str_builder::append(const char c)
    {
        grow(1);
        this->string_value[this->length++] = c;
    }

    void str_builder::append(const char *string)
    {
        unsigned other_string_length = (unsigned) strlen(string);
        unsigned start_position = this->length;
        unsigned new_length = start_position + other_string_length;
        unsigned other_string_counter = 0;
        grow(other_string_length);
        for (unsigned i = start_position; i < new_length; ++i) {
            this->string_value[i] = string[other_string_counter++];
        }
        this->length = new_length;
    }

    char *str_builder::pop()
    {
        char *new_str = new_string(this->string_value);
        free(this->string_value);
        this->reset();
        return new_str;
    }

    void str_builder::reset()
    {
        this->string_value = (char *) new_object_array(INIT_CAPACITY, sizeof(char));
        this->capacity = INIT_CAPACITY;
        this->length = 0;
    }

    str_builder::~str_builder()
    {
        free(this->string_value);
    }

    const char *str_builder::get_string() const
    {
        return this->string_value;
    }

    unsigned str_builder::size() const
    {
        return this->length;
    }
}