cmake_minimum_required(VERSION 3.10)
project(TLM_Nonblock_Blocking)

# Try to find SystemC. It might be installed in a standard location
# or provided via CMAKE_PREFIX_PATH.
find_package(SystemCLanguage CONFIG REQUIRED)

add_executable(tlm_example main.cpp)
target_link_libraries(tlm_example SystemC::systemc)