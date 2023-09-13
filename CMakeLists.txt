cmake_minimum_required(VERSION 3.26)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_pool src/main.cpp
        include/Thread.h
        include/ThreadPool.h
        include/Task.h
        src/Thread.cpp
        src/ThreadPool.cpp
        src/Task.cpp
        include/Any.h
        include/Base.h
        include/Semaphore.h
        src/Semaphore.cpp
        include/Result.h
        src/Result.cpp)
