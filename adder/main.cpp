#include <systemc.h>
#include "adder.h"
#include "driver.h"
#include "monitor.h"

int sc_main(int argc, char* argv[]) {
    // Signals to connect the modules
    sc_signal<sc_uint<4> > sig_a;
    sc_signal<sc_uint<4> > sig_b;
    sc_signal<sc_uint<5> > sig_sum;

    // Instantiate modules
    driver d("driver");
    monitor m("monitor");
    adder dut("adder");

    // Connect modules to signals
    dut.a(sig_a);
    dut.b(sig_b);
    dut.sum(sig_sum);

    m.m_a(sig_a);
    m.m_b(sig_b);
    m.m_sum(sig_sum);

    d.d_a(sig_a);
    d.d_b(sig_b);

    // Setup VCD tracing
    sc_trace_file *tf = sc_create_vcd_trace_file("wave");
    tf->set_time_unit(1, SC_NS);
    sc_trace(tf, sig_a, "A");
    sc_trace(tf, sig_b, "B");
    sc_trace(tf, sig_sum, "Sum");

    std::cout << "Starting simulation..." << std::endl;
    
    // Start the simulation
    sc_start();

    // Close trace file
    sc_close_vcd_trace_file(tf);

    return 0;
}
