#ifndef INTERMEDIARY_H
#define INTERMEDIARY_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

using namespace sc_core;
using namespace sc_dt;

class Intermediary : public sc_module {
public:
    tlm_utils::simple_target_socket<Intermediary> target_socket;
    tlm_utils::simple_initiator_socket<Intermediary> initiator_socket;

    Intermediary(sc_module_name name) : sc_module(name), target_socket("target_socket"), initiator_socket("initiator_socket") {
        target_socket.register_b_transport(this, &Intermediary::b_transport);
        initiator_socket.register_nb_transport_bw(this, &Intermediary::nb_transport_bw);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        std::cout << "@" << sc_time_stamp() << " Intermediary: Received b_transport request" << std::endl;
        
        // Consume requested delay
        wait(delay);
        delay = SC_ZERO_TIME;
        
        tlm::tlm_phase phase = tlm::BEGIN_REQ;
        sc_time fw_delay = SC_ZERO_TIME;
        
        std::cout << "@" << sc_time_stamp() << " Intermediary: Forwarding with nb_transport_fw" << std::endl;
        tlm::tlm_sync_enum status = initiator_socket->nb_transport_fw(trans, phase, fw_delay);
        
        if (status == tlm::TLM_ACCEPTED || status == tlm::TLM_UPDATED) {
            // Need to wait for BEGIN_RESP
            wait(response_event);
        } else if (status == tlm::TLM_COMPLETED) {
            // Transaction completed immediately
        }
        
        std::cout << "@" << sc_time_stamp() << " Intermediary: b_transport request completed" << std::endl;
    }

    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& delay) {
        if (phase == tlm::BEGIN_RESP) {
            std::cout << "@" << sc_time_stamp() << " Intermediary: Received nb_transport_bw (BEGIN_RESP)" << std::endl;
            
            // Acknowledge the response
            phase = tlm::END_RESP;
            
            // Notify b_transport to resume
            response_event.notify();
            
            return tlm::TLM_COMPLETED;
        }
        return tlm::TLM_ACCEPTED;
    }

private:
    sc_event response_event;
};

#endif // INTERMEDIARY_H
