#ifndef THREAD_POOL_THREADPOOL_H
#define THREAD_POOL_THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Thread.h"
#include "Task.h"

namespace ThreadPool
{
	// 线程池类型
	enum class ThreadPoolMode
	{
		MODE_FIXED, // 固定数量的线程
		MODE_CACHED // 线程数量可动态增长
	};

	// 线程池类型（单例模式）
	class ThreadPool
	{
	public:
		// 得到线程池对象（单例模式）
		static ThreadPool& getInstance()
		{
			static ThreadPool instance;
			return instance;
		}


		// 设置线程池的工作模式
		void setMode(ThreadPoolMode mode);

		// 设置task任务队列上线的阈值
		void setMaxTaskSize(size_t size);

		// 给线程池提交任务
		void submitTask(std::shared_ptr<Task> ptr);

		// 开启线程池（初始线程数默认为4）
		// todo 将size设置为获取CPU核心的线程数
		void start(size_t size = 4);

		~ThreadPool();

	private:
		ThreadPool();

		// 删除拷贝构造函数
		ThreadPool(const ThreadPool&) = delete;

		// 删除赋值运算重载
		ThreadPool& operator=(const ThreadPool&) = delete;

		// 定义线程函数（处理任务队列中的任务）
		void threadFunc();

	private:
		std::vector<Thread*> m_threads; // 线程列表
		std::size_t m_initThreadsSize; // 初始的线程数量
		ThreadPoolMode m_poolMode; // 线程池的工作模式
		std::queue<std::shared_ptr<Task>> m_tasks; // 任务队列
		std::atomic_uint m_taskSize; // 任务的数量
		std::size_t m_maxTaskSize; // 任务队列数量的阈值
		std::mutex m_queueMutex; // 任务队列锁，保证任务队列的线程安全
		std::condition_variable m_notFull; // 表示任务队列不满，可以放任务进去
		std::condition_variable m_notEmpty; // 表示任务队列不空，线程池的线程就可以对任务队列进行处理
	};
}

#endif
