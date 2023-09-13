#ifndef THREAD_POOL_SEMAPHORE_H
#define THREAD_POOL_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

namespace ThreadPool
{
	class Semaphore
	{
	public:
		explicit Semaphore(int limit);

		~Semaphore() = default;

		// 获取一个信号量资源
		void wait();

		// 增加一个信号量资源
		void post();

	private:
		int m_resLimit;
		std::mutex m_mutex;
		std::condition_variable m_cond;
	};
}

#endif