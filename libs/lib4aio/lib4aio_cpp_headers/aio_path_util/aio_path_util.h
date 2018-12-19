#ifndef LIB4AIO_CPP_AIO_PATH_H
#define LIB4AIO_CPP_AIO_PATH_H

namespace lib4aio {

    class str_hook;

    char *construct_absolute_path(const str_hook *relative_path, const char *script_path);

    char *construct_absolute_path(const char *relative_path, const char *script_path);
}

#endif //LIB4AIO_CPP_AIO_PATH_H