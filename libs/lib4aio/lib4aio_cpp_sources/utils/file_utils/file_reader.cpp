#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>
#include <lib4aio_cpp_headers/utils/str_builder/str_builder.h>
#include <lib4aio_cpp_headers/utils/error_utils/error_utils.h>
#include <lib4aio_cpp_headers/utils/string_utils/suffix_prefix.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

#define LIB4AIO_CPP_FILE_READER_DEBUG

#define LIB4AIO_CPP_FILE_READER_ERROR_TAG "LIB4AIO_CPP_FILE_READER_ERROR"

#ifdef LIB4AIO_CPP_FILE_READER_DEBUG

#include <lib4aio_cpp_headers/utils/log_utils/log_utils.h>

#define LIB4AIO_CPP_FILE_READER_INFO_TAG "LIB4AIO_CPP_FILE_READER_INFO"

#endif

namespace lib4aio {

#define READ "r"

#define FILE_READER_TAG "FILE_READER"

    str_builder *read_file_by_str_builder(const char *path)
    {
        str_builder *string_builder = new str_builder();
        //파일을 만들다:
        FILE *file = fopen(path, READ);
        if (file) {
            while (true) {
                const char current_char = (char) fgetc(file);
                if (current_char != EOF) {
                    string_builder->append(current_char);
                } else {
                    break;
                }
            }
            fclose(file);
            return string_builder;
        } else {
            throw_error_with_tag(LIB4AIO_CPP_FILE_READER_ERROR_TAG, "소스 파일을 열 수 없습니다 (Cannot open source file).");
        }
    }

    str_hook *get_name_by_file_path(const char *path, const char *file_format)
    {
        if (ends_with_suffix(path, file_format)) {
            const unsigned last_path_index = (const unsigned) (strlen(path) - 1);
            for (unsigned i = last_path_index; i >= 0; --i) {
                const char symbol = path[i];
                //파일 이름 되다 (Pass file name):
                if (is_slash(symbol)) {
                    const unsigned offset = i + 1;
                    const unsigned length = last_path_index - i - (unsigned) strlen(file_format);
                    str_hook *file_name = new str_hook(path, offset, offset + length);
                    if (file_name->is_word()) {
#ifdef LIB4AIO_CPP_FILE_READER_DEBUG
                        log_info_str_hook(FILE_READER_TAG, "컨텍스트 이름 (Context name):", file_name);
#endif
                        return file_name;
                    } else {
                        throw_error_with_tag(
                                FILE_READER_TAG,
                                "*.aio 파일 이름을 잘못되었습니다 (*.aio file name is invalid)!"
                        );
                    }
                }
            }
            throw_error_with_tag(
                    FILE_READER_TAG,
                    "*.aio 파일 이름을 잘못되었습니다 (*.aio file name is invalid)!"
            );
        } else {
            throw_error_with_tag(
                    FILE_READER_TAG,
                    "파일 형식이 'aio' 가 아닙니다 (File runtime_type is not 'aio')!"
            );
        }
    }
}