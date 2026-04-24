#ifndef ADDER_H
#define ADDER_H

#include <systemc.h>

SC_MODULE(full_adder) {
    sc_in<bool> a, b, cin;
    sc_out<bool> sum, cout;

    void do_add() {
        sum.write(a.read() ^ b.read() ^ cin.read());
        cout.write((a.read() & b.read()) | (b.read() & cin.read()) | (a.read() & cin.read()));
    }

    SC_CTOR(full_adder) {
        SC_METHOD(do_add);
        sensitive << a << b << cin;
    }
};

#endif
