#ifndef O_LEFT_SERVER_H
#define O_LEFT_SERVER_H

class o_left_server {

public:

    void start();

    int middle_server_input_channel = 0;

    int middle_server_output_channel = 0;

    int output_right_anonymous_gate = 0;

    int input_right_anonymous_gate = 0;
};

#endif //O_LEFT_SERVER_H