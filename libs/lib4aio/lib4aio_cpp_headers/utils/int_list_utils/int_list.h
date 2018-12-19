#ifndef INT_LIST_H
#define INT_LIST_H

namespace lib4aio {

    typedef struct int_list {
        unsigned capacity;
        unsigned size;
        int *values;
    } int_list;

    int_list *new_int_list();

    void add_int_in_list(int_list *list, const int value);

    void free_int_list(int_list *list);
}

#endif //INT_LIST_H