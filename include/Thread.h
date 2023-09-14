#ifndef THREAD_POOL_THREAD_H
#define THREAD_POOL_THREAD_H

#include <functional>
#include <thread>

namespace ThreadPool
{
// 线程类型
	class Thread
	{
	public:
		// 给线程函数的方法换个名称
		using ThreadFunc = std::function<void(int)>;

		// 线程构造函数
		explicit Thread(ThreadFunc func);

		// 线程析构函数
		~Thread();

		// 启动线程
		void start();

		// 获取线程ID
		[[nodiscard]] int getThreadId() const;

	private:
		ThreadFunc m_func; // 线程函数类型的对象
		static int m_generateID; // 全局变量，用于标识某个线程
		int m_threadId; // 线程ID
	};
}

#endif
