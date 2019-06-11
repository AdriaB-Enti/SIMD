#pragma once
#include <immintrin.h>
#include "Unoptimised.h"
#include <functional>

Unoptimised::Unoptimised()
{
}

Unoptimised::~Unoptimised()
{
}

void Unoptimised::Add(float px, float py, float vx, float vy) {
	Particle p = Particle(px, py, vx, vy);
	particles.push_back(p);
}

void Unoptimised::Update(float dt) {

	//old code
	for (auto i = particles.begin(); i !=particles.end(); i++)
	{
		i->px += i->vx*dt;
		i->py += i->vy*dt;
	}
}

void Unoptimised::Run(std::function<void(float x, float y)>f) {

	for (auto i = particles.begin(); i != particles.end(); ++i)
	{
		f(i->px, i->py);
	}

}

void Unoptimised::test(){
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
