#ifndef LIB4AIO_STR_BUILDER_H
#define LIB4AIO_STR_BUILDER_H

namespace lib4aio {

    class str_builder {

    public:

        str_builder();

        void append(const char c);

        void append(const char *string);

        char *pop();

        void reset();

        const char *get_string() const;

        unsigned size() const;

        ~str_builder();

    private:

        char *string_value;

        unsigned capacity;

        unsigned length;

        void grow(const unsigned range);
    };

}

#endif //LIB4AIO_STR_BUILDER_H