#ifndef DRIVER_H
#define DRIVER_H

#include <systemc.h>

SC_MODULE(driver) {
    sc_out<bool> d_a;
    sc_out<bool> d_b;
    sc_out<bool> d_cin;
    sc_in<bool> d_sum;
    sc_in<bool> d_cout;

    void generate_inputs() {
        // Initial values
        d_a.write(false);
        d_b.write(false);
	d_cin.write(false);
        wait(5, SC_NS);
	sc_assert(d_sum.read() == false);
	sc_assert(d_cout.read() == false);
        
        // Test case 1
        d_a.write(true);
        d_b.write(false);
	d_cin.write(false);
        wait(5, SC_NS);
	sc_assert(d_sum.read() == true);
	sc_assert(d_cout.read() == false);
        
        // Test case 2
        d_a.write(true);
        d_b.write(true);
	d_cin.write(false);
        wait(5, SC_NS);
	sc_assert(d_sum.read() == false);
	sc_assert(d_cout.read() == true);

        // Test case 3
        d_a.write(true);
        d_b.write(true);
	d_cin.write(true);
        wait(5, SC_NS);
	sc_assert(d_sum.read() == true);
	sc_assert(d_cout.read() == true);

        // End simulation
        sc_stop();
    }

    SC_CTOR(driver) {
        SC_THREAD(generate_inputs);
    }
};

#endif
