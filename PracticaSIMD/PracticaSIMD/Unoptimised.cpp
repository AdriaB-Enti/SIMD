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

void Unoptimised::Add(float px, float py, float vx, float vy, float ax = 0.1f, float ay = -0.1f) {
	Particle p = Particle(px, py, vx, vy, ax, ay);
	particles.push_back(p);
}

void Unoptimised::Update(float dt) {

	for (int p = 0; p < particles.size(); p++)
	{
		particles[p].vx += particles[p].ax*dt;
		particles[p].vy += particles[p].ay*dt;
		particles[p].px += particles[p].vx*dt;
		particles[p].py += particles[p].vy*dt;
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
	std::cout << "first value " << particles.front().px << ":" << particles.front().py << std::endl;
	std::cout << "final value " << particles.back().px << ":" << particles.back().py << std::endl;
}
