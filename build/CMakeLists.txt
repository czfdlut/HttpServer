cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")
set(CMAKE_C_COMPILER "/home/chenzhifeng/bin/gcc-4.9.4/bin/gcc")                                                                              
set(CMAKE_CXX_COMPILER "/home/chenzhifeng/bin/gcc-4.9.4/bin/g++")

project(HttpServer)
set(EXECUTABLE_OUTPUT_PATH ${HttpServer_SOURCE_DIR}/bin/)
add_definitions(-g -D _LINUX64)
add_definitions(-std=c++11)
include_directories(
../src
../util
../dependency/output/libevent/linux/include
../dependency/output/libcurl/linux2/include
)

link_directories(
${HttpServer_SOURCE_DIR}/../dependency/output/libevent/linux/lib
${HttpServer_SOURCE_DIR}/../dependency/output/libcurl/linux2/lib
)

file(GLOB ALL_HEADER_SRCS
../src/*.h
../util/*.h
../dependency/output/libevent/linux/include/*.h
../dependency/output/libcurl/linux2/include/*.h
)

file(GLOB ALL_CXX_SRCS
../src/*.cpp
../util/*.cpp
)

set(ALL_LIB dl pthread libevent.a rt curl ssl crypto)
add_executable(HttpServer ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(HttpServer ${ALL_LIB})

