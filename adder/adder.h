#ifndef ADDER_H
#define ADDER_H

#include <systemc.h>

SC_MODULE(adder) {
    sc_in<sc_uint<4> > a;
    sc_in<sc_uint<4> > b;
    sc_out<sc_uint<5> > sum;

    void do_add() {
        sum.write(a.read() + b.read());
    }

    SC_CTOR(adder) {
        SC_METHOD(do_add);
        sensitive << a << b;
    }
};

#endif
