#pragma once
#include "E19AdriaBiarnes.h"
#include <immintrin.h>
#include <thread>

E19AdriaBiarnes::E19AdriaBiarnes()
{

}

void E19AdriaBiarnes::Add(float px, float py, float vx, float vy) {
	Particle p = Particle(px, py, vx, vy);
	particles.push_back(p);
}

void E19AdriaBiarnes::Update(float dt) {

	int totalParticles = particles.size();

	auto i = particles.begin();

	__m128 valuesA = _mm_set_ps(dt, dt, dt, dt);

	for (int part = 0; part < (int)(totalParticles / 2); part++)	//iterate every 2 particles
	{
		Particle p1 = Particle(i->px, i->py, i->vx, i->vy);
		Particle p2 = Particle(std::next(i,1)->px, std::next(i,1)->py, std::next(i,1)->vx, std::next(i,1)->vy);
		
		__m128 valuesB = _mm_set_ps(p1.vx, p1.vy, p2.vx, p2.vy);
		__m128 valuesC = _mm_set_ps(p1.px, p1.py, p2.px, p2.py);

		__m128 result = _mm_fmadd_ps(valuesA, valuesB, valuesC);	//A * B + C

		i->px = result.m128_f32[3];
		i->py = result.m128_f32[2];

		++i;

		i->px = result.m128_f32[1];
		i->py = result.m128_f32[0];

		++i;
	}

	//remainig particles
	while (i != particles.end())
	{
		i->px += i->vx*dt;
		i->py += i->vy*dt;

		++i;
	}

	//old code
	/*for (auto i = particles.begin(); i !=particles.end(); i++)
	{
		i->px += i->vx*dt;
		i->py += i->vy*dt;
	}*/
}

void E19AdriaBiarnes::test() {
	StopWatch watch = StopWatch();

	float t0 = watch.time();
	std::cout << t0 << "\n";

	for (int i = 0; i < 1000000; i++)
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

	for (auto i = particles.begin(); i != particles.end(); ++i)
	{
		f(i->px, i->py);
	}
}
