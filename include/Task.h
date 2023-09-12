#ifndef THREAD_POOL_TASK_H
#define THREAD_POOL_TASK_H

namespace ThreadPool
{
	// 任务抽象基类
	class Task
	{
	public:
		virtual void run() = 0;
	};
}

#endif
