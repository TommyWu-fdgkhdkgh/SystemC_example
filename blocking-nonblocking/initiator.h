#ifndef INITIATOR_H
#define INITIATOR_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

using namespace sc_core;
using namespace sc_dt;

class Initiator : public sc_module {
public:
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_HAS_PROCESS(Initiator);

    Initiator(sc_module_name name) : sc_module(name), socket("socket") {
        SC_THREAD(thread_process);
    }

    void thread_process() {
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        sc_time delay = sc_time(10, SC_NS);
        
        int data = 42;

        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_address(0x0);
        trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
        trans->set_data_length(4);
        trans->set_streaming_width(4);
        trans->set_byte_enable_ptr(0);
        trans->set_dmi_allowed(false);
        trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        std::cout << "@" << sc_time_stamp() << " Initiator: Sending blocking WRITE request, data=" << data << std::endl;

        socket->b_transport(*trans, delay);

        if (trans->is_response_error()) {
            SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
        }

        std::cout << "@" << sc_time_stamp() << " Initiator: Blocking WRITE request completed, delay=" << delay << std::endl;
        
        wait(delay); // realize the delay
        
        delete trans;
    }
};

#endif // INITIATOR_H
