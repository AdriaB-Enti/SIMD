#pragma once
#include "E19AdriaBiarnes.h"
#include <thread>

E19AdriaBiarnes::E19AdriaBiarnes()
{
	lastAddedIndex = 0;
	positions = std::vector<__m128>();
	speeds = std::vector<__m128>();
	accelerations = std::vector<__m128>();
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
}

void E19AdriaBiarnes::Update(float dt) {

	__m128 deltaVector = _mm_set_ps1(dt);
	
	auto v = speeds.begin();
	auto a = accelerations.begin();
	for (auto p = positions.begin(); p != positions.end(); p++, v++, a++)
	{
		*v = _mm_fmadd_ps(deltaVector, *a, *v);
		*p = _mm_fmadd_ps(deltaVector, *v, *p);
	}
}

void E19AdriaBiarnes::test() {
	StopWatch watch = StopWatch();

	float t0 = watch.time();
	std::cout << t0 << "\n";

	for (int i = 0; i < 50000; i++)
	{
		//Add(rand() * 10, rand() * 10, rand() * 3, rand() * 3);
		Add(1.1f, 1.1f, 0.1f, 0.1f);
	}

	float t1 = watch.time();
	std::cout << t1 << "\n";

	for (int i = 0; i < 100000; i++)
	{
		Update(0.3f);
	}


	float t2 = watch.time();
	std::cout << t2 << "\n";

	std::cout << "total time " << t2 - t1 << "\n";
	
	std::cout << "first value1 " << positions.begin()->m128_f32[0] << ":";
	std::cout << positions.begin()->m128_f32[1] << std::endl;
	std::cout << "final value1 " << positions.back().m128_f32[lastAddedIndex*2] << ":";
	std::cout << positions.back().m128_f32[1 + lastAddedIndex * 2] << std::endl;
}

void E19AdriaBiarnes::Run(std::function<void(float x, float y)>f) {
	for (auto p = positions.begin(); p != positions.end(); p++)
	{
		f(p->m128_f32[0], p->m128_f32[1]); // x, y
		if (p < positions.end()-1 || lastAddedIndex != 0) //last element might or might not have 2 particles (but at least 1)
		{
			f(p->m128_f32[2], p->m128_f32[3]);
		}
	}
}
