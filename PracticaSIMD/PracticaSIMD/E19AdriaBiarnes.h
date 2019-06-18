#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <immintrin.h>
#include <queue>
#include <thread>
#include <mutex>

class E19AdriaBiarnes
{
public:

	struct StopWatch {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		float time() { return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start).count(); }
	};

	struct ParticleData
	{
		float x, y;
		ParticleData(float _x, float _y) :x(_x), y(_y) { }
	};

	int lastAddedIndex;

	std::vector<__m128> positions;
	std::vector<__m128> speeds;
	std::vector<__m128> accelerations;
	int particleCount;

	E19AdriaBiarnes();
	~E19AdriaBiarnes();
	void Add(float px, float py, float vx, float vy, float ax, float ay);
	void Update(float dt);
	void Run(std::function<void(float x, float y)>f);
	void test();

	//Threads
	struct Task
	{
		__m128 dt = _mm_set_ps1(0);
		std::vector<__m128>::iterator posStart, velStart, accStart;
		int nPartclPairs = 0;

		Task(std::vector<__m128>::iterator _posStart, std::vector<__m128>::iterator _velStart,
			std::vector<__m128>::iterator _accStart, __m128 _dt, int _nparticles) {
			posStart = _posStart;
			velStart = _velStart;
			accStart = _accStart;
			nPartclPairs = (int) ceil((float) _nparticles / 2.0f);
			dt = _dt;
		}
		Task() {
		}
		void threadUpdate() {
			for (int pp = 0; pp < nPartclPairs; pp++, posStart++, velStart++, accStart++)
			{
				*velStart = _mm_fmadd_ps(dt, *accStart, *velStart);		//doesn't care of odd/even nº of particles
				*posStart = _mm_fmadd_ps(dt, *velStart, *posStart);
			}
		}
	};

	int availableTasks;
	bool threadsActive = true;
	std::queue<Task> tasks;
	std::vector<std::thread> threadPool;

	void threadWorker();
};