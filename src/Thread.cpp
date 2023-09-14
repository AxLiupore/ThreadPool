#include <utility>

#include "../include/Thread.h"

ThreadPool::Thread::Thread(ThreadFunc func) : m_func(std::move(func)), m_threadId(m_generateID++)
{
}

ThreadPool::Thread::~Thread() = default;

// 启动线程
void ThreadPool::Thread::start()
{
	// 创建一个线程去处理函数
	std::thread t(m_func, m_generateID);
	t.detach(); // 设置分离线程
}

int ThreadPool::Thread::m_generateID = 0;

int ThreadPool::Thread::getThreadId() const
{
	return m_threadId;
}


