#ifndef DRIVER_H
#define DRIVER_H

#include <systemc.h>

SC_MODULE(driver) {
    sc_out<sc_uint<4> > d_a;
    sc_out<sc_uint<4> > d_b;

    void generate_inputs() {
        // Initial values
        d_a.write(0);
        d_b.write(0);
        wait(5, SC_NS);
        
        // Test case 1
        d_a.write(5);
        d_b.write(7);
        wait(5, SC_NS);
        
        // Test case 2
        d_a.write(10);
        d_b.write(15);
        wait(5, SC_NS);

        // Test case 3
        d_a.write(15);
        d_b.write(15);
        wait(5, SC_NS);
        
        // End simulation
        sc_stop();
    }

    SC_CTOR(driver) {
        SC_THREAD(generate_inputs);
    }
};

#endif
