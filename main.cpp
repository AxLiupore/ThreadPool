#include <iostream>
#include <chrono>
#include "include/ThreadPool.h"
#include "include/Thread.h"
#include "include/Task.h"

int main()
{
	ThreadPool::ThreadPool& pool = ThreadPool::ThreadPool::getInstance();
	pool.start(10);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return 0;
}