#ifndef O_CLIENT_H
#define O_CLIENT_H

class o_client {

public:

    void start();

private:

    int input_channel = 0;

    int output_channel = 0;

    void ask();

    void call_by_socket(const char *request, const unsigned long size, const int m_socket);

    char *receive_by_socket(const int m_socket);
};

#endif //O_CLIENT_H