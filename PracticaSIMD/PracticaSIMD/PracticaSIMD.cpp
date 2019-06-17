// Adrià Biarnés
// https://ark.intel.com/content/www/us/en/ark/products/88967/intel-core-i7-6700hq-processor-6m-cache-up-to-3-50-ghz.html

#pragma once
#include <iostream>
#include "E19AdriaBiarnes.h"
#include "Unoptimised.h"

//std::mutex m;

//void testFil(std::vector<int>::iterator start, std::vector<int>::iterator end) {
//	for (; start <= end; start++)
//	{
//		//std::cout << *start << std::endl;
//		//m.lock();
//		*start += 50;
//		*start -= 2 + *start / 3;
//		//m.unlock();
//	}
//	std::cout << "this is the thread ENDING \n";
//}
//struct StopWatch {
//	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//	float time() { return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start).count(); }
//};
int main()
{
	const int NTHREADS = 6;
	std::cout << "Testing SIMD exercice\n";
	std::cout << "Threads " << std::thread::hardware_concurrency() << "\n";
	
	bool optimised = true;
	optimised = false;
	
	if (optimised) {
		E19AdriaBiarnes e19 = E19AdriaBiarnes();
		e19.test();
	} else {
		Unoptimised un = Unoptimised();
		un.test();
	}

	/*
	
	std::vector<int> proves = std::vector<int>();
	for (int i = 0; i < 100000000; i++)
	{
		proves.push_back(i);
	}
	StopWatch watch = StopWatch();

	std::thread threads[NTHREADS];

	auto primer = proves.begin();
	auto ultim = proves.end() - 1;

	float t0 = watch.time();

	//NO THREADS
	//testFil(primer, ultim);
	
	int partPerThread = (int) std::ceil(proves.size() / NTHREADS);
	int extraPartThreads = proves.size() % NTHREADS;
	int currentThreadStart = 0;
	//THREADS
	for (int t = 0; t < NTHREADS; t++)
	{
		auto threadStart = proves.begin() + currentThreadStart;
		//int dbgEnd = currentThreadStart + partPerThread - 1 + (t < extraPartThreads ? 1 : 0);
		//std::cout << "thread " << t << " start: " << currentThreadStart << "\t / end: " << dbgEnd << "\t COUNT: " << dbgEnd-currentThreadStart+1 << std::endl;
		if (t < extraPartThreads)
		{
			threads[t] = std::thread(testFil, threadStart, (threadStart + partPerThread + 1));
			currentThreadStart += partPerThread + 1;
		}
		else
		{
			threads[t] = std::thread(testFil, threadStart, (threadStart + partPerThread));
			currentThreadStart += partPerThread;
		}
		//threads[t] = std::thread(testFil, primer, ultim);
	}

	for (int t = 0; t < NTHREADS; t++)
	{
		threads[t].join();
	}
	float t1 = watch.time();
	for (int i = 0; i < 100; i++)
	{
		std::cout << proves[i] << ", ";
	}

	std::cout << "temps final " << t1 - t0 << "\n";
	std::cout << "VALOR FINAL " << proves.back() << "\n";
	*/


	/*struct ParticlePos
	{
		float x, y;
	};
	__m128 valorsA = _mm_set_ps1(-1.0f);
	
	ParticlePos testo;
	testo.x = 1;
	testo.y = 2;

	reinterpret_cast<ParticlePos*>(&valorsA)->x;
	reinterpret_cast<ParticlePos*>(&valorsA)->y;

	reinterpret_cast<ParticlePos*>(&valorsA)[0] = testo;
	reinterpret_cast<ParticlePos*>(&valorsA)[1].x = 3;
	reinterpret_cast<ParticlePos*>(&valorsA)[1].y = 4;
*/


}