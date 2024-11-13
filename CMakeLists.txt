cmake_minimum_required(VERSION 3.29)
project(TraceSysCall)

set(CMAKE_CXX_STANDARD 20)

add_executable(TraceSysCall main.cpp
        Logiciels.h
        SysCall.h)
