#include <systemc>
#include "initiator.h"
#include "intermediary.h"
#include "target.h"

int sc_main(int argc, char* argv[]) {
    Initiator initiator("initiator");
    Intermediary intermediary("intermediary");
    Target target("target");

    // Bindings
    initiator.socket.bind(intermediary.target_socket);
    intermediary.initiator_socket.bind(target.socket);

    std::cout << "Starting simulation..." << std::endl;
    sc_start(100, sc_core::SC_NS);

    return 0;
}
