cmake_minimum_required(VERSION 3.26)
project(thread_pool)

set(CMAKE_CXX_STANDARD 14)

add_executable(thread_pool main.cpp
        include/ThreadPool.h
        include/Task.h
        include/Thread.h
        src/ThreadPool.cpp
        src/Thread.cpp
        src/Task.cpp)
