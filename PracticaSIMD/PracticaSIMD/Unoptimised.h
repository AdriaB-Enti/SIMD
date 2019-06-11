#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <functional>

class Unoptimised
{
public:
	struct Particle
	{
		float px, py, vx, vy;
		Particle(float _px, float _py, float _vx, float _vy) :px(_px), py(_py), vx(_vx), vy(_vy) { }
	};

	struct StopWatch {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		float time() { return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start).count(); }
	};

	std::vector<Particle> particles = std::vector<Particle>();

	Unoptimised();
	void Add(float px, float py, float vx, float vy);
	void Update(float dt);
	void Run(std::function<void(float x, float y)>f);
	void test();
	~Unoptimised();
};

