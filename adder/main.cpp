#include <systemc.h>
#include "adder.h"
#include "driver.h"
#include "monitor.h"

int sc_main(int argc, char* argv[]) {
    // Signals to connect the modules
    sc_signal<bool> sig_a;
    sc_signal<bool> sig_b;
    sc_signal<bool> sig_cin;
    sc_signal<bool> sig_sum;
    sc_signal<bool> sig_cout;

    // Instantiate modules
    driver d("driver");
    monitor m("monitor");
    full_adder dut("adder");

    // Connect modules to signals
    dut.a(sig_a);
    dut.b(sig_b);
    dut.cin(sig_cin);
    dut.sum(sig_sum);
    dut.cout(sig_cout);

    m.m_a(sig_a);
    m.m_b(sig_b);
    m.m_cin(sig_cin);
    m.m_sum(sig_sum);
    m.m_cout(sig_cout);

    d.d_a(sig_a);
    d.d_b(sig_b);
    d.d_cin(sig_cin);
    d.d_sum(sig_sum);
    d.d_cout(sig_cout);

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
