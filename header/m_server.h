#ifndef M_SERVER_H
#define M_SERVER_H

class m_server {

public:

    explicit m_server();

    virtual ~m_server();

    void start();

    void set_input_anonymous_gate(int input_anonymous_gate);

    void set_output_anonymous_gate(int output_anonymous_gate);

    /**
     * Archiver.
     */

    int input_archive_channel = 0;

    int output_archive_channel = 0;

private:

    /**
     * Client.
     */

    int request_channel = 0;

    int response_channel = 0;

    /**
     * Checker.
     */

    int input_anonymous_gate = 0;

    int output_anonymous_gate = 0;

    void launch_service();

    char *handle_request(const char *request);
};

#endif //M_SERVER_H
