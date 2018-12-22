#ifndef M_SERVER_H
#define M_SERVER_H

class m_middle_server {

public:

    explicit m_middle_server();

    virtual ~m_middle_server();

    void start();

private:

    int request_channel = 0;

    int response_channel = 0;

    int input_archive_channel = 0;

    void launch_service();

    char *handle_request(const char *request);
};

#endif //M_SERVER_H
