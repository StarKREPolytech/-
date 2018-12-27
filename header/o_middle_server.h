#ifndef O_MIDDLE_SERVER_H
#define O_MIDDLE_SERVER_H

class o_middle_server {

public:

    void start();

    int input_left_anonymous_gate = 0;

    int output_left_anonymous_gate = 0;

    int input_right_anonymous_gate = 0;

    int output_right_anonymous_gate = 0;

private:

    bool is_syncing = false;

    int output_client_channel = 0;

    int input_client_channel = 0;

    void launch_service();

    void listen_file();

    void sync();
};

#endif //O_MIDDLE_SERVER_H