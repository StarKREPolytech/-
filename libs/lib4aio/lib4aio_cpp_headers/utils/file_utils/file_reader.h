#ifndef LIB4AIO_FILE_READER_H
#define LIB4AIO_FILE_READER_H

namespace lib4aio {

    class str_hook;

    class str_builder;

    str_hook *get_name_by_file_path(const char *path, const char *file_format);

    str_builder *read_file_by_str_builder(const char *path);
}

#endif //LIB4AIO_FILE_READER_H