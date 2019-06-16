#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <immintrin.h>

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

	E19AdriaBiarnes();
	void Add(float px, float py, float vx, float vy, float ax, float ay);
	void Update(float dt);
	void Run(std::function<void(float x, float y)>f);
	void test();
};

