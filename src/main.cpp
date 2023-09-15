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

	pool.setMode(ThreadPool::ThreadPoolMode::MODE_CACHED);
	pool.start(4);

	ThreadPool::Result result1 = pool.submitTask(std::make_shared<MyTask>(1, 100000));
	ThreadPool::Result result2 = pool.submitTask(std::make_shared<MyTask>(1, 100000));
	ThreadPool::Result result3 = pool.submitTask(std::make_shared<MyTask>(1, 100000));
	pool.submitTask(std::make_shared<MyTask>(1, 100000));
	pool.submitTask(std::make_shared<MyTask>(1, 100000));
	pool.submitTask(std::make_shared<MyTask>(1, 100000));

	auto sum1 = result1.get().cast<int>();
	std::cout << sum1 << std::endl;
	auto sum2 = result2.get().cast<int>();
	auto sum3 = result3.get().cast<int>();

	std::cout << sum1 + sum2 + sum3 << std::endl;
	return 0;
}