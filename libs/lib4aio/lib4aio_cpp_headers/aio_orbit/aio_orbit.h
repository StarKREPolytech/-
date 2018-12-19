#ifndef LIB4AIO_CPP_AIO_ORBIT_H
#define LIB4AIO_CPP_AIO_ORBIT_H

#include <functional>

namespace lib4aio {

    class str_hook;

    template<typename T>
    class array_list;

    template<typename T>
    class aio_particle;

    template<typename T>
    class aio_orbit {

    public:

        static aio_orbit<T> *create();

        aio_orbit<T> *set_pivot(T *pivot_ptr);

        aio_orbit<T> *set_radius(const str_hook *radius);

        aio_orbit<T> *set_particle(aio_particle<T> *particle);

        aio_orbit<T> *spin();

        void finish();

    protected:

        array_list<aio_particle<T>> *particle_list;

        T *pivot_ptr;

    private:

        enum {
            AIO_ALL_PARTICLES_SCAN, AIO_ONE_PARTICLE_SCAN
        } particle_mode;

        aio_particle<T> *active_particle_ptr;

        const str_hook *radius;

        unsigned iterator_position;

        void reset_particles();

        void illuminate(T *object);

        explicit aio_orbit();

        virtual ~aio_orbit();
    };
}

#endif //LIB4AIO_CPP_AIO_ORBIT_H