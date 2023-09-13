#ifndef THREAD_POOL_RESULT_H
#define THREAD_POOL_RESULT_H

#include <memory>
#include <atomic>
#include "Any.h"
#include "Semaphore.h"
#include "Task.h"

namespace ThreadPool
{
	class Task;

	class Result
	{
	public:
		explicit Result(std::shared_ptr<Task> task, bool isValid = true);

		~Result() = default;

		// 获取任务执行完的返回值
		void setValue(Any any);

		// 用户调用这个方法获取task的返回值
		Any get();

	private:
		Any m_any; // 存储任务的返回值
		Semaphore m_semaphore = Semaphore(0); // 线程通信信号量
		std::shared_ptr<Task> m_task; // 指向对应获取返回值的任务对象
		std::atomic_bool m_isValid; // 返回值是否有效
	};
}

#endif
