#ifndef INTERMEDIARY_H
#define INTERMEDIARY_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

SC_MODULE(Intermediary) {
    tlm_utils::simple_target_socket<Intermediary> target_socket;
    tlm_utils::simple_initiator_socket<Intermediary> initiator_socket;

    SC_CTOR(Intermediary) : target_socket("target_socket"), initiator_socket("initiator_socket") {
        target_socket.register_b_transport(this, &Intermediary::b_transport);
    }

    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        cout << sc_time_stamp() << " Intermediary: Received transaction, forwarding to target" << endl;
        
        // Add some processing delay
        delay += sc_time(5, SC_NS);

        // Forward the transaction
        initiator_socket->b_transport(trans, delay);

        cout << sc_time_stamp() << " Intermediary: Received response from target, returning to initiator" << endl;
    }
};

#endif