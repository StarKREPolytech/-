namespace lib4aio
{

#ifndef LIB4AIO_STRING_UTILS_SUFFIX_PREFIX_H
#define LIB4AIO_STRING_UTILS_SUFFIX_PREFIX_H

    char *remove_prefix(const char *src, const char *prefix);

    char *remove_suffix(const char *src, const char *suffix);

    char *remove_prefix_suffix(const char *src, const char *prefix, const char *suffix);

    bool starts_with_prefix(const char *src, const char *prefix);

    bool ends_with_suffix(const char *src, const char *suffix);

#endif //LIB4AIO_STRING_UTILS_SUFFIX_PREFIX_H
}