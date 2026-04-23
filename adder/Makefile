CXX = g++

# Adjust this path if SystemC is installed elsewhere
SYSTEMC_HOME ?= $(PWD)/systemc-install

# Try to determine the lib directory based on architecture, fallback to lib
LIB_DIR := $(shell if [ -d $(SYSTEMC_HOME)/lib-linux64 ]; then echo lib-linux64; else echo lib; fi)

CXXFLAGS = -I$(SYSTEMC_HOME)/include -Wno-deprecated
LDFLAGS = -L$(SYSTEMC_HOME)/$(LIB_DIR) -lsystemc

##################
#####  MISC  #####
##################
.PHONY: clean
clean: adder/clean systemc/clean

#####################
#####  example  #####
#####################
.PHONY: adder/build
adder/build: adder

.PHONY: adder/run
adder/run: adder
	export SYSTEMC_HOME=$(SYSTEMC_HOME) && \
            export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/$(LIB_DIR) && \
            ./adder 

adder: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

adder/clean:
	rm -f adder
	rm -f wave.vcd

######################
####  SystemC    #####
######################
.PHONY: systemc/clone
systemc/clone:
	git clone https://github.com/accellera-official/systemc.git
	cd systemc && git checkout 2.3.4

.PHONY: systemc/build
systemc/build:
	mkdir systemc-install
	mkdir systemc/build
	cd systemc/build && cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
	  -DCMAKE_INSTALL_PREFIX=$(PWD)/systemc-install ..
	cd systemc/build && make -j16
	cd systemc/build && make install

.PHONY: systemc/clean
systemc/clean:
	rm -rf ./systemc
	rm -rf ./systemc-install
