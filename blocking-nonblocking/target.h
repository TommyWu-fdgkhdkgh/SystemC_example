#ifndef TARGET_H
#define TARGET_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/peq_with_get.h>

using namespace sc_core;
using namespace sc_dt;

class Target : public sc_module {
public:
    tlm_utils::simple_target_socket<Target> socket;

    SC_HAS_PROCESS(Target);

    Target(sc_module_name name) : sc_module(name), socket("socket"), transaction_peq("transaction_peq") {
        socket.register_nb_transport_fw(this, &Target::nb_transport_fw);
        SC_THREAD(process_transaction);
    }

    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& delay) {
        if (phase == tlm::BEGIN_REQ) {
            std::cout << "@" << sc_time_stamp() << " Target: Received BEGIN_REQ" << std::endl;
            
            // Accept the request and schedule it to be processed
            transaction_peq.notify(trans, delay);
            phase = tlm::END_REQ;
            return tlm::TLM_ACCEPTED;
        } else if (phase == tlm::END_RESP) {
            std::cout << "@" << sc_time_stamp() << " Target: Received END_RESP" << std::endl;
            return tlm::TLM_COMPLETED;
        }
        
        return tlm::TLM_ACCEPTED;
    }

    void process_transaction() {
        while (true) {
            tlm::tlm_generic_payload* trans;
            while ((trans = transaction_peq.get_next_transaction()) == 0) {
                wait(transaction_peq.get_event());
            }

            // Simulate processing time
            wait(20, SC_NS);
            
            trans->set_response_status(tlm::TLM_OK_RESPONSE);

            tlm::tlm_phase bw_phase = tlm::BEGIN_RESP;
            sc_time bw_delay = SC_ZERO_TIME;
            
            std::cout << "@" << sc_time_stamp() << " Target: Sending BEGIN_RESP" << std::endl;
            
            tlm::tlm_sync_enum status = socket->nb_transport_bw(*trans, bw_phase, bw_delay);
            
            if (status == tlm::TLM_COMPLETED || (status == tlm::TLM_ACCEPTED && bw_phase == tlm::END_RESP)) {
                 // Transaction completed
            }
        }
    }

private:
    tlm_utils::peq_with_get<tlm::tlm_generic_payload> transaction_peq;
};

#endif // TARGET_H
