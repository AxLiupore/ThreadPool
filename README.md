# ThreadPool

## 简介

这是基于 C++11 实现的线程池，用于有效的管理和重用线程，以执行异步任务，以减少线程的创建和销毁开销，提高性能和资源利用率

1. 基于可变参模板编程和引用折叠原理，实现线程池 submitTask 接口，支持任意任务函数和任意参数的传递
2. 使用 future 类型制定 submitTask 提交任务的返回值
3. 使用 map 和 queue 容器管理线程对象和任务
4. 基于条件变量 condition_variable 和互斥锁 mutex 实现任务提交线程和任务执行线程间的通信机制
5. 支持 fixed 和 cached 模式的线程池制定

## 编译

- 本工程支持 MacOS，Linux 和 Windows 系统，无需任何第三方依赖，默认使用 C++14 进行编译，不支持 C++11 以下的版本
- 本项目使用 CLion 编译器进行编译，打开 CMakeLists.txt 文件作为工程，即可编译通过
- Linux 环境下，在命令行模式下，输入以下指令，即可编译通过

```bash

```

## 模块

### ThreadPool



