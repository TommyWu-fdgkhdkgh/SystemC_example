#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <queue>
#include <iostream>

using namespace sc_core;
using namespace tlm;

// 1. Initiator Module
SC_MODULE(Initiator) {
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator) : socket("socket") {
        socket.register_nb_transport_bw(this, &Initiator::nb_transport_bw);
        SC_THREAD(thread_process);
    }

    void thread_process() {
        tlm_generic_payload* trans = new tlm_generic_payload;
        sc_time delay = SC_ZERO_TIME;
        
        tlm_phase phase = BEGIN_REQ;
        
        trans->set_command(TLM_WRITE_COMMAND);
        trans->set_address(0x100);
        int data = 42;
        trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
        trans->set_data_length(4);
        trans->set_response_status(TLM_INCOMPLETE_RESPONSE);

        std::cout << sc_time_stamp() << " [Initiator] Sending nb_transport_fw" << std::endl;
        
        tlm_sync_enum status = socket->nb_transport_fw(*trans, phase, delay);
        
        if (status == TLM_ACCEPTED) {
            std::cout << sc_time_stamp() << " [Initiator] Transaction accepted, waiting for response" << std::endl;
        } else if (status == TLM_COMPLETED) {
            std::cout << sc_time_stamp() << " [Initiator] Transaction completed immediately" << std::endl;
            delete trans;
        }
    }

    tlm_sync_enum nb_transport_bw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) {
        if (phase == BEGIN_RESP) {
            std::cout << sc_time_stamp() << " [Initiator] Received nb_transport_bw with BEGIN_RESP. Status: " 
                      << trans.get_response_string() << std::endl;
            
            // End the transaction
            phase = END_RESP;
            delete &trans; // Clean up payload since we are done
            return TLM_COMPLETED;
        }
        return TLM_ACCEPTED;
    }
};

// 2. Intermediary Module
SC_MODULE(Intermediary) {
    tlm_utils::simple_target_socket<Intermediary> target_socket;
    tlm_utils::simple_initiator_socket<Intermediary> initiator_socket;

    std::queue<tlm_generic_payload*> trans_queue;
    sc_event trans_event;

    SC_CTOR(Intermediary) : target_socket("target_socket"), initiator_socket("initiator_socket") {
        target_socket.register_nb_transport_fw(this, &Intermediary::nb_transport_fw);
        SC_THREAD(processing_thread);
    }

    tlm_sync_enum nb_transport_fw(tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) {
        if (phase == BEGIN_REQ) {
            std::cout << sc_time_stamp() << " [Intermediary] Received BEGIN_REQ, queueing transaction" << std::endl;
            trans_queue.push(&trans);
            trans_event.notify(delay); // Notify after any requested delay
            
            phase = END_REQ; // Indicate that request phase is complete
            return TLM_ACCEPTED;
        }
        return TLM_ACCEPTED;
    }

    void processing_thread() {
        while (true) {
            if (trans_queue.empty()) {
                wait(trans_event);
            }
            
            if (trans_queue.empty()) continue; // Safety check

            tlm_generic_payload* trans = trans_queue.front();
            trans_queue.pop();

            std::cout << sc_time_stamp() << " [Intermediary] Forwarding as blocking transport" << std::endl;
            sc_time delay = SC_ZERO_TIME;
            
            initiator_socket->b_transport(*trans, delay);
            wait(delay); // Advance simulation time by the delay returned from b_transport

            std::cout << sc_time_stamp() << " [Intermediary] Blocking transport finished, sending response back" << std::endl;
            
            tlm_phase bw_phase = BEGIN_RESP;
            sc_time bw_delay = SC_ZERO_TIME;
            target_socket->nb_transport_bw(*trans, bw_phase, bw_delay);
        }
    }
};

// 3. Target Module
SC_MODULE(Target) {
    tlm_utils::simple_target_socket<Target> socket;

    SC_CTOR(Target) : socket("socket") {
        socket.register_b_transport(this, &Target::b_transport);
    }

    void b_transport(tlm_generic_payload& trans, sc_time& delay) {
        std::cout << sc_time_stamp() << " [Target] Received b_transport, simulating work" << std::endl;
        
        delay += sc_time(10, SC_NS); // Simulate 10ns of work
        
        trans.set_response_status(TLM_OK_RESPONSE);
        
        // Note: For real models, we don't output anything, but this demonstrates the sequence
        // Wait won't happen here, the delay is added and handled by the caller.
    }
};

int sc_main(int argc, char* argv[]) {
    Initiator init("Initiator");
    Intermediary inter("Intermediary");
    Target target("Target");

    init.socket.bind(inter.target_socket);
    inter.initiator_socket.bind(target.socket);

    std::cout << "Starting Simulation..." << std::endl;
    sc_start(100, SC_NS); // Start simulation
    std::cout << "Simulation Completed." << std::endl;

    return 0;
}