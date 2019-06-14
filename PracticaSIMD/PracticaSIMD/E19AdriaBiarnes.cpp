#pragma once
#include "E19AdriaBiarnes.h"
#include <thread>

E19AdriaBiarnes::E19AdriaBiarnes()
{
	currentIndex = 0;
	positions = std::vector<__m128>();
	speeds = std::vector<__m128>();
	accelerations = std::vector<__m128>();
}

void E19AdriaBiarnes::Add(float px, float py, float vx, float vy, float ax = 0.1f, float ay = -0.1f) {

	if (currentIndex == 0)
	{
		positions.push_back(_mm_set_ps1(0.0f));
		speeds.push_back(_mm_set_ps1(0.0f));
		accelerations.push_back(_mm_set_ps1(0.f));
	}

	//each __m128 contains two particles (and each particle has two floats: x, y)
	reinterpret_cast<ParticleData*>(&positions.back())[currentIndex].x = px;
	reinterpret_cast<ParticleData*>(&positions.back())[currentIndex].y = py;
	reinterpret_cast<ParticleData*>(&speeds.back())[currentIndex].x = vx;
	reinterpret_cast<ParticleData*>(&speeds.back())[currentIndex].y = vy;
	reinterpret_cast<ParticleData*>(&accelerations.back())[currentIndex].x = ax;
	reinterpret_cast<ParticleData*>(&accelerations.back())[currentIndex].y = ay;

	currentIndex = ++currentIndex % 2;
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

	for (int i = 0; i < 500000; i++)
	{
		Add(rand() * 10, rand() * 10, rand() * 3, rand() * 3);
	}

	float t1 = watch.time();
	std::cout << t1 << "\n";

	for (int i = 0; i < 10000; i++)
	{
		Update(0.3f);
	}


	float t2 = watch.time();
	std::cout << t2 << "\n";

	std::cout << "total time " << t2 - t1 << "\n";

}

void E19AdriaBiarnes::Run(std::function<void(float x, float y)>f) {
	//TODO: canviar
	for (auto i = particles.begin(); i != particles.end(); ++i)
	{
		f(i->px, i->py);
	}
}
