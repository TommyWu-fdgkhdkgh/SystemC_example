#ifndef INITIATOR_H
#define INITIATOR_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

SC_MODULE(Initiator) {
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator) : socket("socket") {
        SC_THREAD(thread_process);
    }

    void thread_process() {
        tlm::tlm_generic_payload trans;
        sc_time delay = sc_time(10, SC_NS);
        unsigned char data[4];

        // Write transaction
        data[0] = 0x11; data[1] = 0x22; data[2] = 0x33; data[3] = 0x44;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(0x100);
        trans.set_data_ptr(data);
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(0);
        trans.set_dmi_allowed(false);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        cout << sc_time_stamp() << " Initiator: Sending WRITE to address 0x" << std::hex << trans.get_address() << " with data 0x" 
             << (int)data[0] << (int)data[1] << (int)data[2] << (int)data[3] << std::dec << endl;

        socket->b_transport(trans, delay);

        if (trans.is_response_error()) {
            SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
        } else {
            cout << sc_time_stamp() << " Initiator: WRITE successful" << endl;
        }

        wait(delay); // realize the delay
        
        // Read transaction
        delay = sc_time(10, SC_NS);
        data[0] = 0; data[1] = 0; data[2] = 0; data[3] = 0;
        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_address(0x100);
        trans.set_data_ptr(data);
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(0);
        trans.set_dmi_allowed(false);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        cout << sc_time_stamp() << " Initiator: Sending READ from address 0x" << std::hex << trans.get_address() << std::dec << endl;

        socket->b_transport(trans, delay);

        if (trans.is_response_error()) {
            SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
        } else {
            cout << sc_time_stamp() << " Initiator: READ successful, data = 0x" << std::hex 
                 << (int)data[0] << (int)data[1] << (int)data[2] << (int)data[3] << std::dec << endl;
        }
        
        wait(delay); // realize the delay
    }
};

#endif