#include <lib4aio_cpp_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_cpp_headers/utils/array_list_utils/array_list.h>

#include <iostream>

namespace lib4aio {

#define INIT_CAPACITY 2

    template<class T>
    array_list<T>::array_list(bool holder_mode)
    {
        this->holder_mode = holder_mode;
        this->capacity = INIT_CAPACITY;
        this->size = 0;
        this->elements = (T **) calloc(INIT_CAPACITY, sizeof(T));
    }

    template<class T>
    void array_list<T>::grow()
    {
        if (this->size == this->capacity) {
            this->capacity *= INIT_CAPACITY;
            this->elements = (T **) (realloc(this->elements, this->capacity * sizeof(T)));
        }
    }

    template<class T>
    void array_list<T>::add(T *element)
    {
        this->grow();
        this->elements[this->size++] = element;
    }

    template<typename T>
    inline array_list<T> *array_list<T>::filter(function<bool(const T *)> func)
    {
        for (unsigned i = 0; i < this->size; ++i) {
            T *element = this->elements[i];
            if (!func(element)) {
                this->remove(element);
            }
        }
        return this;
    }

    template<class T>
    inline array_list<T> *array_list<T>::foreach(function<void(T *)> func)
    {
        auto size = this->size;
        auto elements = this->elements;
        for (unsigned i = 0; i < size; ++i) {
            func(elements[i]);
        }
        return this;
    }

    template<typename T>
    inline bool array_list<T>::contains_by(function<bool(const T *)> func) const
    {
        for (unsigned i = 0; i < this->size; ++i) {
            if (func(this->elements[i])) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    inline T *array_list<T>::find_by(function<bool(T *)> func) const
    {
        bool is_found = false;
        T *unique_element = nullptr;
        for (unsigned i = 0; i < this->size; ++i) {
            T *element = this->elements[i];
            if (func(element)) {
                if (!is_found) {
                    is_found = true;
                    unique_element = element;
                } else {
                    unique_element = nullptr;
                }
            }
        }
        return unique_element;
    }

    template<typename T>
    inline array_list<T> *array_list<T>::collect_by(function<bool(T *)> func)
    {
        array_list<T> *collected_list = new array_list<T>();
        for (unsigned i = 0; i < this->size; ++i) {
            T *element = this->elements[i];
            if (func(element)) {
                collected_list->add(element);
            }
        }
        return collected_list;
    }

    template<typename T>
    void array_list<T>::reduce()
    {
        if (this->size == this->capacity / 2) {
            this->capacity /= INIT_CAPACITY;
            this->elements = (T **) (realloc(this->elements, this->capacity * sizeof(T)));
        }
    }

    template<typename T>
    bool array_list<T>::remove(T *element)
    {
        for (unsigned i = 0; i < this->size; ++i) {
            if (this->elements[i] == element) {
                return this->remove(i);
            }
        }
        return false;
    }

    template<typename T>
    bool array_list<T>::remove(const unsigned position)
    {
        if (position <= this->size) {
            T *element = this->elements[position];
            this->elements[position] = nullptr;
            if (this->holder_mode) {
                this->shift_to_left(position);
            } else {
                bool has_references = false;
                for (unsigned i = position + 1; i < this->size; ++i) {
                    if (element == this->elements[i]) {
                        this->shift_to_left(position);
                        has_references = true;
                        break;
                    }
                }
                if (!has_references) {
                    delete element;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    template<typename T>
    void array_list<T>::shift_to_left(const unsigned position)
    {
        this->size--;
        for (unsigned i = position; i < this->size; ++i) {
            this->elements[i] = this->elements[i + 1];
        }
    }

    /**
     * Boilerplate.
     */

    template<class T>
    array_list<T>::~array_list()
    {
        for (unsigned i = 0; i < this->size; ++i) {
            T *element = this->elements[i];
            this->elements[i] = nullptr;
            if (this->holder_mode) {
                delete element;
            }
        }
        delete this->elements;
    }

    template<class T>
    unsigned array_list<T>::get_size() const
    {
        return this->size;
    }

    template<typename T>
    T *array_list<T>::last()
    {
        return this->elements[this->size - 1];
    }


    template<typename T>
    T *&array_list<T>::operator[](const unsigned index)
    {
        std::cout << "[]" << endl;
        return this->elements[index];
    }

    template<typename T>
    T *array_list<T>::get(const unsigned index) const
    {
        return this->elements[index];
    }

    template<typename T>
    inline bool array_list<T>::equals(const array_list<T> *other, function<bool(const T *, const T *)> compare)
    {
        if (this->holder_mode == other->holder_mode
            && this->capacity == other->capacity
            && this->size == other->size) {
            for (unsigned i = 0; i < this->size; ++i) {
                const T *element = this->elements[i];
                const T *other_element = other->elements[i];
                if (!compare(element, other_element)) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }
}