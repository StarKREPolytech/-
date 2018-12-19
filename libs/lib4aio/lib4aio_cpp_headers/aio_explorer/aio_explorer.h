#ifndef AIO_EXPLORER_H
#define AIO_EXPLORER_H

namespace lib4aio {

    class str_hook;

    class aio_explorer {

    public:

        static str_hook *explore_hook_scope(
                const unsigned start,
                const char left_border,
                const char right_border,
                const str_hook *parent_hook_ptr
        );
    };
}

#endif //AIO_EXPLORER_H