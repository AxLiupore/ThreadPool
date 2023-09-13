#include "../include/ThreadPool.h"

const int TASK_MAX_THREADSIZE = 1024;

ThreadPool::ThreadPool::ThreadPool() : m_taskNumber(0),
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
void ThreadPool::ThreadPool::submitTask(const std::shared_ptr<Task>& ptr)
{
	// 获取锁
	std::unique_lock<std::mutex> lock(m_queueMutex);

	// 线程的通信---等待任务队列中可以放任务
	if (!m_notFull.wait_for(lock, std::chrono::seconds(1), [&]() -> bool
	{ return m_threads.size() < m_maxTaskSize; }))
	{
		std::cerr << "task queue is full, submit task fail" << std::endl;
		return;
	}

	// 如果有空余，把任务放入任务队列中
	m_tasks.emplace(ptr);
	m_taskNumber++;
	std::cout << "submit task success" << std::endl;

	// 因为新放了任务，任务队列肯定不空---通知线程去处理任务
	m_notEmpty.notify_all();
}

// 开启线程池
void ThreadPool::ThreadPool::start(size_t size)
{
	m_initThreadsSize = size;

	// 创建线程对象
	for (int i = 0; i < m_initThreadsSize; i++)
	{
		// 创建thread线程对象的时候，把线程函数给到thread线程对象
		m_threads.emplace_back(std::move(std::make_unique<Thread>([this]
		{ threadFunc(); }))); // 接受构造函数所需要的参数，在容器内创建元素，避免不必要的拷贝
	}

	// 启动所有的线程
	for (int i = 0; i < m_initThreadsSize; i++)
	{
		m_threads[i]->start();
	}
}

// 定义线程函数（处理任务队列中的任务）
[[noreturn]] void ThreadPool::ThreadPool::threadFunc()
{
	while (true)
	{
		// 先获取锁
		std::unique_lock<std::mutex> lock(m_queueMutex);

		std::cout << "tid:" << std::this_thread::get_id()
				  << " try to task" << std::endl;
		// 等待notEmpty条件
		m_notEmpty.wait(lock, [&]() -> bool
		{ return !m_tasks.empty(); });

		std::cout << "tid:" << std::this_thread::get_id()
				  << " get task" << std::endl;

		// 从任务队列中取出一个任务出来
		auto task = m_tasks.front();
		m_tasks.pop();
		m_taskNumber--;

		// 如果依然有剩余任务，继续通知其他线程执行任务
		if (!m_tasks.empty())
		{
			m_notEmpty.notify_all();
		}

		// 取出一个任务，进行通知可以继续生产任务
		m_notFull.notify_all();

		// 解锁
		lock.unlock();

		// 当前线程负责执行这个任务
		if (task != nullptr)
		{
			task->run();
			std::cout << "tid:" << std::this_thread::get_id()
					  << " finish this task" << std::endl;
		}
	}
}
