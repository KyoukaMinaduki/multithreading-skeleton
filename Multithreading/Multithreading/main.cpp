#include <thread>
#include <vector>
#include "TSQ.h"

#include <iostream>
#include <chrono>

enum taskType {Yolo, Quit};

void decode(int task);
void scheduler();

TSQ<int> q;

int main()
{
	//init
	int num_thread = 10;
	std::vector<std::thread> threads;
	for (int i = 0; i < num_thread; i++)
	{
		threads.push_back(std::thread(scheduler));
	}

	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

	//execute
	for (int i = 0; i < 1000; i++)
	{
		q.push(Yolo);
	}
	for (int i = 0; i < num_thread; i++)
	{
		q.push(Quit);
	}
	for (int i = 0; i < num_thread; i++)
	{
		threads[i].join();
	}

	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	std::cout << total_time.count() << std::endl;

	return 0;
}

void decode(int task)
{
	switch (task)
	{
		case Yolo:
			std::cout << "Yolo\n";
			break;
		case Quit:
			throw "Quit\n";
		default:
			std::cout << "Default\n";
	}
}

void scheduler()
{
	while (true)
	{
		if (!q.empty())
		{
			int task = q.extract();
			try
			{
				decode(task);
			}
			catch (const char* err)
			{
				std::cout << err;
				return;
			}

		}
	}
}

