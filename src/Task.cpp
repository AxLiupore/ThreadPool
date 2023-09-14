#include "../include/Task.h"

ThreadPool::Task::Task() : m_result(nullptr)
{

}

void ThreadPool::Task::exec()
{
	if (m_result != nullptr)
	{
		m_result->setValue(run()); // 这里发生多态调用，这个run的函数是用户线程的任务
	}
}

void ThreadPool::Task::setResult(ThreadPool::Result* result)
{
	m_result = result;
}