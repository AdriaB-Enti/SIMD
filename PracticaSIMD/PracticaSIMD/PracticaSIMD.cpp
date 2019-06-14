// Adrià Biarnés
// https://ark.intel.com/content/www/us/en/ark/products/88967/intel-core-i7-6700hq-processor-6m-cache-up-to-3-50-ghz.html

#pragma once
#include <iostream>
#include "E19AdriaBiarnes.h"
#include <immintrin.h>
#include "Unoptimised.h"


int main()
{
    std::cout << "Hello, World\n";
	E19AdriaBiarnes exerciciE19 = E19AdriaBiarnes();
	exerciciE19.test();

	/*Unoptimised un = Unoptimised();
	un.test();*/

	/*struct ParticlePos
	{
		float x, y;
	};

	std::cout << "Size of float: " << sizeof(ParticlePos) << " bytes\n";

	__m128 valorsA;

	//valorsA = _mm_set_ps1(-1.0f);
	
	ParticlePos testo;
	testo.x = 1;
	testo.y = 2;

	reinterpret_cast<ParticlePos*>(&valorsA)->x;
	reinterpret_cast<ParticlePos*>(&valorsA)->y;

	reinterpret_cast<ParticlePos*>(&valorsA)[0] = testo;
	reinterpret_cast<ParticlePos*>(&valorsA)[1].x = 3;
	reinterpret_cast<ParticlePos*>(&valorsA)[1].y = 4;

	std::cout << valorsA.m128_f32[0] << std::endl;
	std::cout << valorsA.m128_f32[1] << std::endl;
	std::cout << valorsA.m128_f32[2] << std::endl;
	std::cout << valorsA.m128_f32[3] << std::endl;


	std::vector<__m128> positions = std::vector<__m128>();
	std::vector<__m128> speeds = std::vector<__m128>();

	for (int i = 0; i < 4; i++)
	{
		positions.push_back(_mm_set_ps1(i));
		speeds.push_back(_mm_set_ps1(i-1));
	}

	__m128 delta = _mm_set_ps1(2);

	auto v = speeds.begin();
	for (auto p = positions.begin(); p != positions.end(); p++, v++)
	{
		*p = _mm_fmadd_ps(delta, *v, *p);
	}

	std::cout << "valor: " << positions[3].m128_f32[0]; //ho mostra al revés
	*/

	//	currentIndex = ++currentIndex % 2;

	//__m128 valorsA = _mm_set_ps(2.0f, 2.0f, 2.0f, 3.0f);
	//__m128 valorsB = _mm_set_ps(3.0f, 3.0f, 3.0f, 4.0f);
	//__m128 valorsC = _mm_set_ps(1.0f, 1.0f, 1.0f, 1.0f);

	////std::cout << "valor: " << valors.m128_f32[0]; //ho mostra al revés

	//__m128 result = _mm_fmadd_ps(valorsA, valorsB, valorsC);

	//std::cout << "resultat: " << result.m128_f32[3] << ", " << result.m128_f32[2] << ", " << result.m128_f32[1] << ", " << result.m128_f32[0] << ", \n/n";
}