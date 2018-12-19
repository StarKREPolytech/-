#include <lib4aio_cpp_headers/aio_orbit/aio_particle/aio_particle.h>
#include <lib4aio_cpp_headers/utils/str_hook_utils/str_hook/str_hook.h>

namespace lib4aio {

    template<class S>
    aio_particle<S>::aio_particle()
    {
        this->is_inside_string = false;
        this->token_holder = nullptr;
    }

    template<class S>
    aio_particle<S>::~aio_particle()
    {
        delete this->token_holder;
    }

    template<class S>
    void aio_particle<S>::set_scanning_string_holder(const str_hook *string_holder)
    {
      this->radius_ptr = string_holder;
      this->token_holder = new str_hook(string_holder);
    }

    template<class S>
    void aio_particle<S>::recycle()
    {
        this->token_holder->start = 0;
        this->token_holder->end = 0;
    }
}