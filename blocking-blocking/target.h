#ifndef TARGET_H
#define TARGET_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

SC_MODULE(Target) {
    tlm_utils::simple_target_socket<Target> socket;

    // Simple memory
    unsigned char mem[256];

    SC_CTOR(Target) : socket("socket") {
        socket.register_b_transport(this, &Target::b_transport);
        for (int i = 0; i < 256; ++i) mem[i] = 0;
    }

    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address();
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        // Check for unsupported features
        if (byt != 0 || len > 4 || wid < len) {
            trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
            return;
        }

        if (adr >= 256) {
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }

        // Add some target processing delay
        delay += sc_time(15, SC_NS);

        cout << sc_time_stamp() << " Target: Processing " << (cmd == tlm::TLM_READ_COMMAND ? "READ" : "WRITE") 
             << " at address 0x" << std::hex << adr << std::dec << endl;

        // Perform read or write
        if (cmd == tlm::TLM_READ_COMMAND) {
            memcpy(ptr, &mem[adr], len);
        } else if (cmd == tlm::TLM_WRITE_COMMAND) {
            memcpy(&mem[adr], ptr, len);
        }

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
};

#endif