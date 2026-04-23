#ifndef MONITOR_H
#define MONITOR_H

#include <systemc.h>

SC_MODULE(monitor) {
    sc_in<sc_uint<4> > m_a;
    sc_in<sc_uint<4> > m_b;
    sc_in<sc_uint<5> > m_sum;

    void print_data() {
        std::cout << "@ " << sc_time_stamp() 
                  << " | A: " << m_a.read() 
                  << " | B: " << m_b.read() 
                  << " | Sum: " << m_sum.read() << std::endl;
    }

    SC_CTOR(monitor) {
        SC_METHOD(print_data);
        // Trigger print when any of these signals change
        sensitive << m_a << m_b << m_sum;
    }
};

#endif
