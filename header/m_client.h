#ifndef M_CLIENT_H
#define M_CLIENT_H

#include <functional>

using namespace std;

namespace lib4aio {

    class str_builder;
}

class m_client {

public:

    explicit m_client();

    virtual ~m_client();

    void start();


private:

    bool is_started = false;

    long last_modified_file;

    void listen_file();

    void call(const char *request, const unsigned size);

    char *receive();
};

#endif //M_CLIENT_H