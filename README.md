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

### 项目架构设计

![architecture](https://github.com/AxLiupore/ThreadPool/blob/master/images/architecture.jpg)

### ThreadPool

#### 线程池的模式

线程池需要支持两种类型，定义一个枚举类型 ThreadPoolMode 来设置线程池的类型

```cpp
enum class ThreadPoolMode
{
    MODE_FIXED, 
    MODE_CACHED 
};
```

#### 线程池

用户传入进来的任务生命周期可能在任务还没处理的时候就没了，这时候就会使用未定义的资源产生报错。为了拉长对象的生命周期，使用智能指针对任务进行管理；应该将线程池设置为单例模式，因为不同的对象操作线程，就会有线程安全问题

#### 线程函数

![threadfunc](https://github.com/AxLiupore/ThreadPool/blob/master/images/threadfunc.jpg)

线程函数是处理任务队列中的任务，当在创建 Thread 对象的时候，将要处理的线程函数通过绑定器绑定到 Thread 类上，然后这 Thread 创建一个 thread 去处理这个线程函数；这个线程函数里面一直循环处理任务，直到线程池对象销毁

通过以下几个步骤循环执行任务

1. 先获取锁
2. 等待`notEmpty` 条件不为空
3. 从任务队列中取出一个任务出来
4. 如果依然有剩余任务，继续通知其他线程执行任务
5. 通知`notFull` 任务队列不满了，可以继续生产任务
6. 将当前锁解锁
7. 当前线程负责执行这个任务

#### 提交任务

用户通过 ThreadPool 里面的`submitTask` 函数将要处理的任务交给线程池去处理

提交任务要保持线程安全，所以通过以下步骤保证线程安全并且完美的实现提交任务

1. 先获取任务队列的锁，将任务队列锁起来
2. 线程通信，等待任务队列有空余就把任务放到任务队列中去
3. 因为新放了任务，所以任务队列不为空，就通过线程去出去任务

### Thread 

这是线程池里面的线程，用于处理线程池中的任务，当线程池有任务的时候，通过线程函数`threadFunc` 去处理这个任务；在开线程去处理线程函数的时候，需要将线程分离开来，这样就能避免线程对象销毁导致程序出现错误

#### 执行

Thread 类接受 ThreadPool 传递过来的线程函数，通过`start` 函数去执行要处理的线程函数

### Task 

因为用户传入的任务不知道是什么类型，所以要定义一个通用的类型去接受不同的任务

定义一个任务抽象基类，用户可以自定义任意任务类型，从`Task`类继承，重写`run`方法，实现自定义任务处理

```cpp
class Task
{
public:
    virtual void run() = 0;
};
```

### Any

![threadfunc](https://github.com/AxLiupore/ThreadPool/blob/master/images/any.jpg)

要让线程池去执行要处理的任务，需要继承这个 Task 类，用户的`run` 方法可能的返回值类型都是不同的，需要设计一个任意类型 Any 去返回，这个类能指向其他任意类型；有一个 Base 类型，派生出一个 Derive 类型，通过将数据类型传递到 Derive 类型实现可接受任意类型的类型
