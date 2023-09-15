#include "../include/ThreadPool.h"

const int TASK_MAX_SIZE = 1024; // 任务的最大数量
const int THREAD_MAX_SIZE = 512; // 线程的最大数量
const int THREAD_MAX_IDLE_TIME = 60; // 线程空闲最大的超时时间

ThreadPool::ThreadPool::ThreadPool() : m_taskNumber(0),
									   m_initThreadNumber(4),
									   m_maxTaskSize(TASK_MAX_SIZE),
									   m_poolMode(ThreadPoolMode::MODE_FIXED),
									   m_isRunning(false),
									   m_idleThreadNumber(0),
									   m_maxThreadSize(THREAD_MAX_SIZE),
									   m_threadNumber(0)
{

}

ThreadPool::ThreadPool::~ThreadPool()
{
	m_isRunning = false;
	m_notEmpty.notify_all(); // 告诉所有线程不空
	// 等待线程池里面所有的线程返回，两种状态：阻塞、正在执行任务中
	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_exitCond.wait(lock, [&]() -> bool
	{ return m_tasks.empty(); }); // 线程为空了
}

// 设置线程池的工作模式
void ThreadPool::ThreadPool::setMode(ThreadPoolMode mode)
{
	if (!checkTurnOn())
	{
		m_poolMode = mode;
	}
}

// 设置task任务队列上线的阈值
void ThreadPool::ThreadPool::setMaxTaskSize(size_t size)
{
	if (!checkTurnOn())
	{
		m_maxTaskSize = size;
	}
}

// 设置空闲线程的最大阈值
void ThreadPool::ThreadPool::setMaxThreadSize(size_t size)
{
	if (!checkTurnOn())
	{
		m_maxThreadSize = size;
	}
}

// 给线程池提交任务
ThreadPool::Result ThreadPool::ThreadPool::submitTask(const std::shared_ptr<Task>& ptr)
{
	// 获取锁
	std::unique_lock<std::mutex> lock(m_queueMutex);

	// 线程的通信---等待任务队列中可以放任务
	if (!m_notFull.wait_for(lock, std::chrono::seconds(1), [&]() -> bool
	{ return m_threads.size() < m_maxTaskSize; }))
	{
		std::cerr << "task queue is full, submit task fail" << std::endl;
		return Result(ptr, false);
	}

	// 如果有空余，把任务放入任务队列中
	m_tasks.emplace(ptr);
	m_taskNumber++;

	// 因为新放了任务，任务队列肯定不空---通知线程去处理任务
	m_notEmpty.notify_all();

	// cached模式下：根据任务数量和空闲线程的数量，判断是否需要创建新的线程出来
	if (m_poolMode == ThreadPoolMode::MODE_CACHED &&
		m_taskNumber > m_idleThreadNumber &&
		m_threadNumber < m_maxThreadSize)
	{
		std::cout << "create new Thread" << std::endl;
		// 创建新线程
		auto thread = std::make_unique<Thread>([this](auto&& PH1)
		{ threadFunc(std::forward<decltype(PH1)>(PH1)); });
		auto threadId = thread->getThreadId();
		m_threads.emplace(threadId, std::move(thread));
		m_threads[threadId]->start();
		// 修改线程数量相关的变量
		m_idleThreadNumber++;
		m_threadNumber++;
	}

// 返回任务的Result对象
	return Result(ptr, true);
}

// 开启线程池
void ThreadPool::ThreadPool::start(size_t size)
{
	m_isRunning = true; // 设置线程池的运行状态

	m_initThreadNumber = size;
	m_threadNumber = size; // 设置当前线程数

	// 创建线程对象
	for (int i = 0; i < m_initThreadNumber; i++)
	{
		// 创建thread线程对象的时候，把线程函数给到thread线程对象
		auto thread = std::make_unique<Thread>([this](auto&& PH1)
		{ threadFunc(std::forward<decltype(PH1)>(PH1)); }); // 接受构造函数所需要的参数，在容器内创建元素，避免不必要的拷贝
		auto threadId = thread->getThreadId();
		m_threads.emplace(threadId, std::move(thread));
	}

// 启动所有的线程
	for (int i = 0; i < m_initThreadNumber; i++)
	{
		m_threads[i]->start(); // 需要去执行一个线程函数
		m_idleThreadNumber++; // 记录初始空闲线程的数量
	}
}

// 定义线程函数（处理任务队列中的任务）
void ThreadPool::ThreadPool::threadFunc(int threadId)
{
	auto lastTime = std::chrono::high_resolution_clock::now(); // 上一次执行的时间

	while (checkRunning())
	{
		// 先获取锁
		std::unique_lock<std::mutex> lock(m_queueMutex);

		std::cout << "tid:" << std::this_thread::get_id()
				  << " try to task" << std::endl;

		// cached模式下：有可能已经创建了很多线程，但是空闲时间超过了60s,应该把多余的线程回收掉
		while (m_tasks.empty())
		{
			if (m_poolMode == ThreadPoolMode::MODE_CACHED)
			{
				// 条件变量超时返回
				if (std::cv_status::timeout == m_notEmpty.wait_for(lock, std::chrono::seconds(1)))
				{
					auto now = std::chrono::high_resolution_clock::now();
					auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);
					if (dur.count() >= THREAD_MAX_IDLE_TIME && m_threadNumber > m_initThreadNumber)
					{
						// 开始回收当前线程
						m_threads.erase(threadId);
						m_idleThreadNumber--;
						m_threadNumber--;
						std::cout << "threadID:" << std::this_thread::get_id() << " exit!" << std::endl;
						return;
					}
				}
			}
			else
			{
				// 等待notEmpty条件
				m_notEmpty.wait(lock);
			}
			// 线程要结束，回收线程资源
			if (!checkRunning())
			{
				m_threads.erase(threadId);
				std::cout << "threadID:" << std::this_thread::get_id() << " exit!" << std::endl;
				m_exitCond.notify_all();
				return;
			}
		}
		m_idleThreadNumber--;
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
		// 执行任务：把任务的返回值通过setValue方法给到Result
		task->exec();
		m_idleThreadNumber++;
		lastTime = std::chrono::high_resolution_clock::now(); // 更新线程执行完任务的时间
	}

	m_threads.erase(threadId);
	std::cout << "threadID:" << std::this_thread::get_id() << " exit!" << std::endl;
}

bool ThreadPool::ThreadPool::checkTurnOn() const
{
	return m_isRunning;
}

bool ThreadPool::ThreadPool::checkRunning() const
{
	return m_isRunning;
}
