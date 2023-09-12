#ifndef THREAD_POOL_TASK_H
#define THREAD_POOL_TASK_H

namespace ThreadPool
{
	// 任务抽象基类
	class Task
	{
	public:
		// 用户可以自定义任意任务类型，从Task继承，重写run方法，实现自定义任务处理
		virtual void run() = 0;
	};
}

#endif
