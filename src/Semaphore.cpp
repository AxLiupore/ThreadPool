#include "../include/Semaphore.h"

ThreadPool::Semaphore::Semaphore(int limit) : m_resLimit(limit)
{

}

void ThreadPool::Semaphore::wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	// 等待信号量有资源，没有资源的话，会阻塞当前线程
	m_cond.wait(lock, [&]() -> bool
	{ return m_resLimit > 0; });
	m_resLimit--;
}

void ThreadPool::Semaphore::post()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_resLimit++;
	m_cond.notify_all();
}
