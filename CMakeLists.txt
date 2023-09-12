cmake_minimum_required(VERSION 3.26)
project(thread_pool)

set(CMAKE_CXX_STANDARD 14)

add_executable(thread_pool main.cpp
        include/Thread.h
        include/ThreadPool.h
        include/Task.h
        src/Thread.cpp
        src/ThreadPool.cpp
        src/Task.cpp)
