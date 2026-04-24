#ifndef MONITOR_H
#define MONITOR_H

#include <systemc.h>

SC_MODULE(monitor) {
    sc_in<bool> m_a;
    sc_in<bool> m_b;
    sc_in<bool> m_cin;
    sc_in<bool> m_sum;
    sc_in<bool> m_cout;

    void print_data() {
        std::cout << "@ " << sc_time_stamp() 
                  << " | A: " << m_a.read() 
                  << " | B: " << m_b.read() 
                  << " | Cin: " << m_cin.read()
                  << " | Sum: " << m_sum.read()
                  << " | Cout: " << m_cout.read() << std::endl;
    }

    SC_CTOR(monitor) {
        SC_METHOD(print_data);
        // Trigger print when any of these signals change
        sensitive << m_a << m_b << m_cin << m_sum << m_cout;
    }
};

#endif
