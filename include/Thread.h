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
		using ThreadFunc = std::function<void()>;

		// 线程构造函数
		explicit Thread(ThreadFunc func);

		// 线程析构函数
		~Thread();

		// 启动线程
		void start();

	private:
		ThreadFunc m_func; // 线程函数类型的对象
	};
}

#endif
