#include "../include/Thread.h"

ThreadPool::Thread::Thread(ThreadFunc func) : m_func(std::move(func))
{

}

ThreadPool::Thread::~Thread() = default;

// 启动线程
void ThreadPool::Thread::start()
{
	// 创建一个线程去处理函数
	std::thread t(m_func);
	t.detach(); // 设置分离线程
}
