#pragma once
#include "E19AdriaBiarnes.h"

std::mutex mutex;

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
	for (int t = 0; t < std::thread::hardware_concurrency() - 1; t++) //std::thread::hardware_concurrency() - 1
	{
		threadPool.push_back(std::thread(&E19AdriaBiarnes::threadWorker, this));
	}
}

E19AdriaBiarnes::~E19AdriaBiarnes(){
	mutex.lock();
	threadsActive = false;
	mutex.unlock();
	for (int t = 0; t < threadPool.size(); t++)
	{
		threadPool[t].join();
	}
}

void E19AdriaBiarnes::threadWorker() {
	Task currentTask = Task();
	bool working = false;
	while (threadsActive)
	{
		mutex.lock();
		if (!tasks.empty())
		{
			currentTask = tasks.front();
			tasks.pop();
			working = true;
		}
		mutex.unlock();
		if (working)
		{
			currentTask.threadUpdate();
			mutex.lock();
			availableTasks--;
			mutex.unlock();
			working = false;
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

	//TODO: make every task (except last one) have always an even number of particles
	//int partPerThread = (int)std::ceil((float)particleCount / (float)threadPool.size());
	int partPerThread = particleCount / threadPool.size();
	if (partPerThread % 2 != 0) //each thread mus have an even number of particles
	{
		partPerThread++;
	}

	int currentThreadStart = 0;
	int remainingParticles = particleCount;

	for (int t = 0; t < threadPool.size(); t++)
	{
		//currentThreadStart uses nº of particles, but vector elements work in pairs of 2 particles each
		std::vector<__m128>::iterator posStart = positions.begin() + currentThreadStart/2;
		std::vector<__m128>::iterator velStart = speeds.begin() + currentThreadStart/2;
		std::vector<__m128>::iterator accStart = accelerations.begin() + currentThreadStart/2;

		Task task;

		if (t == threadPool.size()-1)
		{
			task = Task(posStart, velStart, accStart, deltaVector, remainingParticles);
			remainingParticles = 0;

			mutex.lock();
			tasks.push(task);
			availableTasks++;
			mutex.unlock();
		}
		else
		{
			task = Task(posStart, velStart, accStart, deltaVector, partPerThread);
			remainingParticles -= partPerThread;
			currentThreadStart += partPerThread;

			mutex.lock();
			tasks.push(task);
			availableTasks++;
			mutex.unlock();
		}


		if (remainingParticles <= 0)
		{
			break;
		}
	}

	while (true)	//wait until all tasks are finished
	{
		if (availableTasks == 0)
		{
			break;
		}
		std::this_thread::yield();
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

	for (int i = 0; i < 80000; i++) //50000
	{
		//Add(rand() * 10, rand() * 10, rand() * 3, rand() * 3);
		Add(1.1f, 1.1f, 0.1f, 0.1f);
	}

	float t1 = watch.time();
	std::cout << t1 << "\n";

	for (int i = 0; i < 80000; i++) //100000
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