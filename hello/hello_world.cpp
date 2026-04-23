#include <systemc.h>
#include <iostream>

SC_MODULE(HelloWorld) {
    SC_CTOR(HelloWorld) {
        SC_METHOD(say_hello);
    }

    void say_hello() {
        std::cout << "Hello, World!" << std::endl;
    }
};

int sc_main(int argc, char* argv[]) {
    HelloWorld hello("hello");
    sc_start();
    return 0;
}
