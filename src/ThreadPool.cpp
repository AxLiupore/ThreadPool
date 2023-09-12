#include "../include/ThreadPool.h"

const int TASK_MAX_THREADSIZE = 1024;

ThreadPool::ThreadPool::ThreadPool() : m_taskSize(0),
									   m_initThreadsSize(4),
									   m_maxTaskSize(TASK_MAX_THREADSIZE),
									   m_poolMode(ThreadPoolMode::MODE_FIXED)
{

}

ThreadPool::ThreadPool::~ThreadPool() = default;

// 设置线程池的工作模式
void ThreadPool::ThreadPool::setMode(ThreadPoolMode mode)
{
	m_poolMode = mode;
}

// 设置task任务队列上线的阈值
void ThreadPool::ThreadPool::setMaxTaskSize(size_t size)
{
	m_maxTaskSize = size;
}

// 给线程池提交任务
void ThreadPool::ThreadPool::submitTask(std::shared_ptr<Task> ptr)
{

}

// 开启线程池
void ThreadPool::ThreadPool::start(size_t size)
{
	m_initThreadsSize = size;

	// 创建线程对象
	for (int i = 0; i < m_initThreadsSize; i++)
	{
		// 创建thread线程对象的时候，把线程函数给到thread线程对象
		m_threads.emplace_back(new Thread(std::bind(&ThreadPool::threadFunc, this))); // 接受构造函数所需要的参数，在容器内创建元素，避免不必要的拷贝
	}

	// 启动所有的线程
	for (int i = 0; i < m_initThreadsSize; i++)
	{
		m_threads[i]->start();
	}
}

void ThreadPool::ThreadPool::threadFunc()
{

}
