#include <lib4aio_cpp_headers/aio_orbit/aio_orbit.h>
#include <lib4aio_cpp_headers/utils/array_list_utils/array_list.h>
#include <lib4aio_cpp_headers/aio_orbit/aio_particle/aio_particle.h>
#include <lib4aio_cpp_sources/aio_orbit/aio_particle/aio_particle.cpp>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

/**
 * 태그들.
 */

#define LIB4AIO_CPP_ORBIT_INFO_TAG "ORBIT_INFO"

#define LIB4AIO_CPP_ORBIT_DEBUG

#ifdef LIB4AIO_CPP_ORBIT_DEBUG

#include <lib4aio_cpp_headers/utils/log_utils/log_utils.h>

#endif

namespace lib4aio {

    template<typename T>
    aio_orbit<T>::aio_orbit()
    {
        this->particle_mode = AIO_ALL_PARTICLES_SCAN;
        this->particle_list = new array_list<aio_particle<T>>(true);
        //Set to zero:
        this->radius = nullptr;
    }

    template<typename T>
    aio_orbit<T> *aio_orbit<T>::create()
    {
        return new aio_orbit<T>();
    }

    template<typename T>
    aio_orbit<T> *aio_orbit<T>::set_pivot(T *pivot_ptr)
    {
        this->pivot_ptr = pivot_ptr;
        return this;
    }

    template<typename T>
    aio_orbit<T> *aio_orbit<T>::set_radius(const str_hook *radius)
    {
        this->radius = radius;
        return this;
    }

    template<typename T>
    aio_orbit<T> *aio_orbit<T>::set_particle(aio_particle<T> *particle)
    {
        this->particle_list->add(particle);
        return this;
    }

    template<typename T>
    void aio_orbit<T>::reset_particles()
    {
        const unsigned list_size = particle_list->get_size();
        //Provide parent to children:
        for (unsigned i = 0; i < list_size; ++i) {
            aio_particle<T> *particle = particle_list->get(i);
            particle->recycle();
        }
    }

    template<typename T>
    void aio_orbit<T>::illuminate(T *object)
    {
        this->iterator_position = this->active_particle_ptr->illuminate(object);
        this->particle_mode = AIO_ALL_PARTICLES_SCAN;
        this->active_particle_ptr = nullptr;
        this->reset_particles();
#ifdef LIB4AIO_CPP_ORBIT_DEBUG
        log_info(LIB4AIO_CPP_ORBIT_INFO_TAG, "Illuminating is complete!");
#endif
    }

    template<typename T>
    aio_orbit<T> *aio_orbit<T>::spin()
    {
#ifdef LIB4AIO_CPP_ORBIT_DEBUG
        log_info(LIB4AIO_CPP_ORBIT_INFO_TAG, "Launch orbit...");
#endif
        const unsigned list_size = this->particle_list->get_size();
        //문자열 소유자를 놓다:
        this->particle_list->foreach([this](aio_particle<T> *it) {
            it->set_scanning_string_holder(this->radius);
        });
        //스캔 시작:
        this->iterator_position = this->radius->start;
        while (this->iterator_position < this->radius->end) {
            if (this->particle_mode == AIO_ALL_PARTICLES_SCAN) {
                for (unsigned i = 0; i < list_size; ++i) {
                    aio_particle<T> *particle = this->particle_list->get(i);
                    const aio_particle_signal signal = particle->handle_symbol(this->iterator_position);
                    if (signal == AIO_PARTICLE_SIGNAL_DETECTED) {
                        this->active_particle_ptr = particle;
                        this->particle_mode = AIO_ONE_PARTICLE_SCAN;
                        break;
                    }
                    if (signal == AIO_PARTICLE_SIGNAL_IS_READY) {
                        this->illuminate(this->pivot_ptr);
                    }
                }
            } else {
                aio_particle<T> *particle = this->active_particle_ptr;
                aio_particle_signal signal = particle->handle_symbol(this->iterator_position);
                if (signal == AIO_PARTICLE_SIGNAL_IS_READY) {
                    this->active_particle_ptr = particle;
                    this->illuminate(this->pivot_ptr);
                }
            }
            this->iterator_position++;
        }
        //--------------------------------------------------------------------------------------------------------------
        return this;
    }

    template<typename T>
    aio_orbit<T>::~aio_orbit()
    {
        delete this->particle_list;
        delete this->radius;
    }

    template<typename T>
    void aio_orbit<T>::finish()
    {
        delete this;
    }
}