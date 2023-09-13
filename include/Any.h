#ifndef THREAD_POOL_ANY_H
#define THREAD_POOL_ANY_H

#include <memory>
#include "Base.h"

namespace ThreadPool
{
	// Any：可以接受任意数据的类型
	class Any
	{
	public:
		Any() = default;

		~Any() = default;

		Any(const Any&) = delete;

		Any& operator=(const Any&) = delete;

		Any(Any&&) = default;

		Any& operator=(Any&&) = default;

		// 这个构造函数可以让Any类型接受任意其他的数据
		template<typename T>
		explicit Any(T data) : m_base(std::make_unique<Derive<T>>(data))
		{
		}

		// 这个方法能把Any对象存储的数据提取出来
		template<typename T>
		T cast()
		{
			// 基类的指针 => 派生类指针
			auto pd = dynamic_cast<Derive<T>*>(m_base.get());
			if (pd == nullptr)
			{
				throw "type is unmatched";
			}
			return pd->m_data;
		}

	private:
		// 定义一个基类的指针，可以指向派生类
		std::unique_ptr<Base> m_base;
	};
}

#endif
