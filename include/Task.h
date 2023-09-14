#ifndef THREAD_POOL_TASK_H
#define THREAD_POOL_TASK_H

#include "Any.h"
#include "Result.h"

namespace ThreadPool
{
	class Result;

	// 任务抽象基类
	class Task
	{
	public:
		Task();

		~Task() = default;

		// 用户可以自定义任意任务类型，从Task继承，重写run方法，实现自定义任务处理
		virtual Any run() = 0;

		void exec();

		// 设置返回值
		void setResult(Result* result);

	private:
		Result* m_result; // Result对象的生命周期 > Task的生命周期
	};
}

#endif
