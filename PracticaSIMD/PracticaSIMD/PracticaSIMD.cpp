// Adrià Biarnés
// https://ark.intel.com/content/www/us/en/ark/products/88967/intel-core-i7-6700hq-processor-6m-cache-up-to-3-50-ghz.html

#pragma once
#include <iostream>
#include "E19AdriaBiarnes.h"
#include "Unoptimised.h"


int main()
{
	std::cout << "Testing SIMD exercice\n";
	
	bool optimised = true;
	//optimised = false;
	
	if (optimised) {
		E19AdriaBiarnes e19 = E19AdriaBiarnes();
		e19.test();
	} else {
		Unoptimised un = Unoptimised();
		un.test();
	}
}