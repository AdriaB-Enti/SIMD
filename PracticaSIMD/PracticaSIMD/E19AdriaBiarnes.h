#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <immintrin.h>

class E19AdriaBiarnes
{
public:

	struct ParticleData
	{
		float x, y;
	};

	struct ParticleVel
	{
		float vx, vy;
	};

	struct Particle
	{
		float px, py, vx, vy;
		Particle(float _px, float _py, float _vx, float _vy) :px(_px), py(_py), vx(_vx), vy(_vy) { }
	};
	
	int currentIndex;

	std::vector<Particle> particles = std::vector<Particle>();

	std::vector<__m128> positions;
	std::vector<__m128> speeds;
	std::vector<__m128> accelerations;

	struct StopWatch {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		float time() { return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start).count(); }
	};
	E19AdriaBiarnes();
	void Add(float px, float py, float vx, float vy, float ax, float ay);
	void Update(float dt);
	void Run(std::function<void(float x, float y)>f);
	void test();
};

