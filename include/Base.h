#ifndef THREAD_POOL_BASE_H
#define THREAD_POOL_BASE_H

namespace ThreadPool
{
	// 基类类型
	class Base
	{
	public:
		virtual ~Base() = default;
	};

	// 派生类类型
	template<typename T>
	class Derive : public Base
	{
	public:
		explicit Derive(T data) : m_data(data)
		{
		}

		T m_data;
	};
}

#endif
