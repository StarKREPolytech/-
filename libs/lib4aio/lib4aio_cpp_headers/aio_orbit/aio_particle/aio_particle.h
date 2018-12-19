#ifndef LIB4AIO_CPP_AIO_PARTICLE_H
#define LIB4AIO_CPP_AIO_PARTICLE_H

namespace lib4aio {

    class str_hook;

    template<typename T>
    class aio_orbit;

    enum aio_particle_signal {
        AIO_PARTICLE_SIGNAL_UNDEFINED, AIO_PARTICLE_SIGNAL_DETECTED, AIO_PARTICLE_SIGNAL_IS_READY
    };

    enum aio_particle_trigger_mode {
        AIO_TRIGGER_MODE_PASSIVE, AIO_TRIGGER_MODE_ACTIVE
    };

    template<typename T>
    class aio_particle {

    public:

        aio_particle();

        void set_scanning_string_holder(const str_hook *string_holder);

        virtual ~aio_particle();

        virtual const aio_particle_signal handle_symbol(const unsigned position) = 0;

        virtual unsigned illuminate(T *container) = 0;

        virtual void recycle();

    protected:

        const str_hook *radius_ptr;

        str_hook *token_holder;

        aio_particle_signal signal;

        bool is_inside_string;

        aio_particle_trigger_mode trigger_mode;
    };
}

#endif //LIB4AIO_CPP_AIO_PARTICLE_H