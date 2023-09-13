#include "../include/Result.h"

ThreadPool::Result::Result(std::shared_ptr<Task> task, bool isValid) : m_task(std::move(task)), m_isValid(isValid)
{
	m_task->setResult(this);
}

// 用户调用
ThreadPool::Any ThreadPool::Result::get()
{
	if (!m_isValid)
	{
		return (Any&&)("");
	}
	m_semaphore.wait(); // task任务如果没有执行完，这里会阻塞用户的线程
	return std::move(m_any);
}

// 线程调用
void ThreadPool::Result::setValue(ThreadPool::Any any)
{
	// 存储task的返回值
	m_any = std::move(any);
	m_semaphore.post(); // 已经获取了任务的返回值，增加信号量资源
}
