#include <lib4aio_cpp_headers/utils/point_watcher/point_watcher.h>

namespace lib4aio
{

    lib4aio::point_watcher::point_watcher()
    {
        this->reset();
    }

    void point_watcher::reset()
    {
        this->start = 0;
        this->end = 0;
        this->pointer = 0;
        this->mode = POINT_WATCHER_PASSIVE_MODE;
    }

    point_watcher::~point_watcher()
    {}

};