#ifndef M_ARCHIVER_H
#define M_ARCHIVER_H

class m_archiver {

public:

    void start();

    void set_output_anonymous_gate(int output_anonymous_gate);

    void set_input_anonymous_gate(int input_anonymous_gate);

    int input_server_channel;

    int output_server_channel;

private:


    int input_anonymous_gate;

    int output_anonymous_gate;
};

#endif //M_ARCHIVER_H
