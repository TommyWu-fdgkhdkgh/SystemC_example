#include "initiator.h"
#include "intermediary.h"
#include "target.h"

int sc_main(int argc, char* argv[]) {
    Initiator init("Initiator");
    Intermediary inter("Intermediary");
    Target target("Target");

    // Bind sockets
    init.socket.bind(inter.target_socket);
    inter.initiator_socket.bind(target.socket);

    cout << "Starting multi-hop TLM 2.0 simulation" << endl;
    sc_start();
    cout << "Simulation completed" << endl;

    return 0;
}