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







	//__m128 valorsA = _mm_set_ps(2.0f, 2.0f, 2.0f, 3.0f);
	//__m128 valorsB = _mm_set_ps(3.0f, 3.0f, 3.0f, 4.0f);
	//__m128 valorsC = _mm_set_ps(1.0f, 1.0f, 1.0f, 1.0f);

	////std::cout << "valor: " << valors.m128_f32[0]; //ho mostra al revés

	//__m128 result = _mm_fmadd_ps(valorsA, valorsB, valorsC);

	//std::cout << "resultat: " << result.m128_f32[3] << ", " << result.m128_f32[2] << ", " << result.m128_f32[1] << ", " << result.m128_f32[0] << ", \n/n";



}