#include <iostream>
#include <thread>
#include <vector>
#include<numeric>
#include "TestThread.h"

using namespace std;


void GetSumT(vector<int>::iterator first, vector<int>::iterator last, int& result)
{
	result = accumulate(first, last, 0); //调用C++标准库算法
}

void TestThread::testThread()
{
	int result1, result2, result3, result4, result5;
	vector<int> largeArrays;
	for (int i = 0; i < 100000000; i++)
	{
		if (i % 2 == 0)
			largeArrays.push_back(i);
		else
			largeArrays.push_back(-1 * i);
	}
	thread first(GetSumT, largeArrays.begin(),
		largeArrays.begin() + 20000000, std::ref(result1)); //子线程1
	thread second(GetSumT, largeArrays.begin() + 20000000,
		largeArrays.begin() + 40000000, std::ref(result2)); //子线程2
	thread third(GetSumT, largeArrays.begin() + 40000000,
		largeArrays.begin() + 60000000, std::ref(result3)); //子线程3
	thread fouth(GetSumT, largeArrays.begin() + 60000000,
		largeArrays.begin() + 80000000, std::ref(result4)); //子线程4
	thread fifth(GetSumT, largeArrays.begin() + 80000000,
		largeArrays.end(), std::ref(result5)); //子线程5

	first.join(); //主线程要等待子线程执行完毕
	second.join();
	third.join();
	fouth.join();
	fifth.join();

	int resultSum = result1 + result2 + result3 + result4 + result5; //汇总各个子线程的结果
	cout << "resultSum="<<resultSum << endl;
}
std::thread::id main_thread_id = std::this_thread::get_id();
void hello()
{
	std::cout << "Hello Concurrent World\n";
	if (main_thread_id == std::this_thread::get_id())
		std::cout << "This is the main thread.\n";
	else
		std::cout << "This is not the main thread.\n";
}

void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended\n";
}

void TestThread::test1()
{
	hello();
	thread t(hello);
	cout << t.hardware_concurrency() << endl;
	t.join();
	thread a(hello);
	a.detach();
	thread threads[5];

	std::cout << "Spawning 5 threads...\n";
	for (int i = 0; i < 5; ++i)
		threads[i] = thread(pause_thread, i + 1);   // move-assign threads
	std::cout << "Done spawning threads. Now waiting for them to join:\n";
	for (auto& thread : threads)
		thread.join();
	std::cout << "All threads joined!\n";


}

