#include <iostream>
#include <chrono>
#include "../include/ThreadPool.h"
#include "../include/Thread.h"
#include "../include/Task.h"
#include "../include/Any.h"

class MyTask : public ThreadPool::Task
{
public:
	MyTask(int begin, int end) : m_begin(begin), m_end(end)
	{
	}

	ThreadPool::Any run() override
	{
		std::cout << "tid:" << std::this_thread::get_id()
				  << " begin." << std::endl;
		int sum = 0;
		for (int i = m_begin; i <= m_end; i++)
		{
			sum += i;
		}
		std::cout << "tid:" << std::this_thread::get_id()
				  << " end." << std::endl;
		return (ThreadPool::Any)sum;
	}

private:
	int m_begin;
	int m_end;
};

int main()
{
	ThreadPool::ThreadPool& pool = ThreadPool::ThreadPool::getInstance();

	pool.start(4);

	for (int i = 0; i < 10; i ++)
	{
		ThreadPool::Result result = pool.submitTask(std::make_shared<MyTask>(1, 100));
		int sum = result.get().cast<int>();
		std::cout << "sum = " << sum << std::endl;
	}

	getchar();
	return 0;
}