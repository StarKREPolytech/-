#ifndef LIB4AIO_CPP_ARRAY_LIST_H
#define LIB4AIO_CPP_ARRAY_LIST_H

#include <functional>

namespace lib4aio {

    using namespace std;

    template<typename T>
    class array_list {

    public:

        array_list(bool holder_mode);

        ~array_list();

        unsigned get_size() const;

        T *get(const unsigned index) const;

        void add(T *element);

        bool remove(T *element);

        bool remove(const unsigned position);

        inline array_list<T> *filter(function<bool(const T *)> func);

        inline array_list<T> *foreach(function<void(T *)> func);

        inline bool contains_by(function<bool(const T *)> func) const;

        inline T *find_by(function<bool(T *)> func) const;

        inline array_list<T> *collect_by(function<bool(T *)> func);

        class array_list_iterator {

        public:

            explicit array_list_iterator(T **ptr) : p_element(ptr)
            {}

            array_list_iterator operator++()
            {
                array_list_iterator i = *this;
                p_element++;
                return i;
            }

            const array_list_iterator operator++(int junk)
            {
                p_element++;
                return *this;
            }

            T *&operator*()
            {
                return *p_element;
            }

            T **operator->()
            {
                return p_element;
            }

            bool operator==(const array_list_iterator &rhs)
            {
                return p_element == rhs.p_element;
            }

            bool operator!=(const array_list_iterator &rhs)
            {
                return p_element != rhs.p_element;
            }

        private:
            T **p_element;
        };

        T *last();

        T *&operator[](const unsigned index);

        array_list_iterator begin()
        {
            return array_list_iterator(this->elements);
        }

        array_list_iterator end()
        {
            return array_list_iterator(this->elements + this->size);
        }

        class const_array_list_iterator {
        public:

            explicit const_array_list_iterator(T **ptr) : p_element(ptr)
            {}

            const_array_list_iterator operator++()
            {
                const_array_list_iterator i = *this;
                p_element++;
                return i;
            }

            const const_array_list_iterator operator++(int junk)
            {
                p_element++;
                return *this;
            }

            T *&operator*()
            {
                return *p_element;
            }

            const T **operator->()
            {
                return p_element;
            }

            bool operator==(const const_array_list_iterator &rhs)
            {
                return p_element == rhs.p_element;
            }

            bool operator!=(const const_array_list_iterator &rhs)
            {
                return p_element != rhs.p_element;
            }

        private:
            T **p_element;
        };

        const_array_list_iterator begin() const
        {
            return const_array_list_iterator(this->elements);
        }

        const_array_list_iterator end() const
        {
            return const_array_list_iterator(this->elements + this->size);
        }

        inline bool equals(const array_list<T> *other, function<bool(const T *, const T *)> compare);

    private:

        bool holder_mode;

        unsigned capacity;

        unsigned size;

        T **elements;

        void grow();

        void reduce();

        void shift_to_left(const unsigned position);
    };
}

#endif //LIB4AIO_CPP_ARRAY_LIST_H