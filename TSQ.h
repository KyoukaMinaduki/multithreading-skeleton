#pragma once
#include <queue>
#include <shared_mutex>

template<typename T>
class ThreadSafeQueue {
public:
	void push(T item)
	{
		std::unique_lock<std::shared_mutex> lock(this->m);
		this->queue.push(item);
	}
	void pop()
	{
		std::unique_lock<std::shared_mutex> lock(this->m);
		this->queue.pop();
	}
	T front()
	{
		std::shared_lock<std::shared_mutex> lock(this->m);
		return this->queue.front();
	}
	bool empty()
	{
		std::shared_lock<std::shared_mutex> lock(this->m);
		return this->queue.empty();
	}
	T extract()
	{
		T temp;
		{
			std::shared_lock<std::shared_mutex> lock(this->m);
			temp = queue.front();
		}
		{
			std::unique_lock<std::shared_mutex> lock(this->m);
			queue.pop();
		}
		return temp;
	}
private:
	std::queue<T> queue;
	std::shared_mutex m;
};

template<typename T>
using TSQ = ThreadSafeQueue<T>;