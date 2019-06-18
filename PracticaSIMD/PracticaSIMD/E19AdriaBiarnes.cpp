#pragma once
#include "E19AdriaBiarnes.h"

E19AdriaBiarnes::E19AdriaBiarnes()
{
	lastAddedIndex = 0;
	particleCount = 0;
	positions = std::vector<__m128>();
	speeds = std::vector<__m128>();
	accelerations = std::vector<__m128>();
	threadsActive = true;

	availableTasks = 0;
	tasks = std::queue<Task>();
	for (int t = 0; t < 2; t++) //std::thread::hardware_concurrency() - 1
	{
		threadPool.push_back(std::thread(&E19AdriaBiarnes::threadWorker, this));
	}
}

E19AdriaBiarnes::~E19AdriaBiarnes(){
	threadsActive = false;
	for (int t = 0; t < threadPool.size(); t++)
	{
		threadPool[t].join();
	}
}

void E19AdriaBiarnes::threadWorker() {
	std::mutex mutex;
	Task currentTask = Task();
	while (threadsActive)
	{
		mutex.lock();
		if (!tasks.empty())
		{
			currentTask = tasks.front();
			tasks.pop();
		}
		mutex.unlock();
		if (currentTask.started != false)
		{
			currentTask.threadUpdate();
			currentTask.started = false;
			mutex.lock();
			availableTasks--;
			mutex.unlock();
		}
	}
}

void E19AdriaBiarnes::Add(float px, float py, float vx, float vy, float ax = 0.1f, float ay = -0.1f) {
	if (lastAddedIndex == 0)
	{
		positions.push_back(_mm_set_ps1(0.0f));
		speeds.push_back(_mm_set_ps1(0.0f));
		accelerations.push_back(_mm_set_ps1(0.f));
	}

	//each __m128 contains two particles (and each particle has two floats: x, y)
	reinterpret_cast<ParticleData*>(&positions.back())[lastAddedIndex] = ParticleData(px, py);
	reinterpret_cast<ParticleData*>(&speeds.back())[lastAddedIndex] = ParticleData(vx, vy);
	reinterpret_cast<ParticleData*>(&accelerations.back())[lastAddedIndex] = ParticleData(ax, ay);
	
	lastAddedIndex = ++lastAddedIndex % 2;
	particleCount++;
}

void E19AdriaBiarnes::Update(float dt) {

	__m128 deltaVector = _mm_set_ps1(dt);

	std::mutex mutex;

	//TODO: make every task (except last one) have always an even number of particles
	int partPerThread = (int)std::ceil((float)particleCount / (float)threadPool.size());
	int extraPartThreads = particleCount % threadPool.size();
	int currentThreadStart = 0;

	int remainingParticles = 0;

	for (int t = 0; t < threadPool.size(); t++)
	{
		std::vector<__m128>::iterator posStart = positions.begin() + currentThreadStart;
		std::vector<__m128>::iterator velStart = speeds.begin() + currentThreadStart;
		std::vector<__m128>::iterator accStart = accelerations.begin() + currentThreadStart;

		Task task;
		if (t < extraPartThreads)
		{
			task = Task(posStart, velStart, accStart, deltaVector, partPerThread + 1);
			currentThreadStart += partPerThread + 1;
		}
		else
		{
			task = Task(posStart, velStart, accStart, deltaVector, partPerThread);
			currentThreadStart += partPerThread;
		}
		mutex.lock();
		tasks.push(task);
		availableTasks++;
		mutex.unlock();
	}

	while (true)	//wait until all tasks are finished
	{
		//TODO: use lock_guard
		mutex.lock();
		if (availableTasks == 0)
		{
			mutex.unlock();
			break;
		}
		mutex.unlock();
	}

	/*auto v = speeds.begin();
	auto a = accelerations.begin();
	for (auto p = positions.begin(); p != positions.end(); p++, v++, a++)
	{
		*v = _mm_fmadd_ps(deltaVector, *a, *v);
		*p = _mm_fmadd_ps(deltaVector, *v, *p);
	}*/
}

void E19AdriaBiarnes::test() {
	StopWatch watch = StopWatch();

	float t0 = watch.time();
	std::cout << t0 << "\n";

	for (int i = 0; i < 4; i++) //50000
	{
		//Add(rand() * 10, rand() * 10, rand() * 3, rand() * 3);
		Add(1.1f, 1.1f, 0.1f, 0.1f);
	}

	float t1 = watch.time();
	std::cout << t1 << "\n";

	for (int i = 0; i < 1; i++) //100000
	{
		Update(0.3f);
	}


	float t2 = watch.time();
	std::cout << t2 << "\n";

	std::cout << "total time " << t2 - t1 << "\n";
	
	std::cout << "first value " << positions.begin()->m128_f32[0] << ":";
	std::cout << positions.begin()->m128_f32[1] << std::endl;
	//std::cout << "LAST INDEX " << lastAddedIndex << "\n";
	std::cout << "final value " << positions.back().m128_f32[lastAddedIndex == 1 ? 0 : 2] << ":";
	std::cout << positions.back().m128_f32[lastAddedIndex == 1 ? 1 : 3] << std::endl;
}

void E19AdriaBiarnes::Run(std::function<void(float x, float y)>f) {
	for (auto p = positions.begin(); p != positions.end(); p++)
	{
		f(p->m128_f32[0], p->m128_f32[1]); // x, y
		if (p < positions.end()-1 || lastAddedIndex == 0) //last element might or might not have 2 particles (but at least 1)
		{
			f(p->m128_f32[2], p->m128_f32[3]);
		}
	}
}