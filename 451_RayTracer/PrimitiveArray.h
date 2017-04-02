#pragma once
#include "Primitive.h"
#include <vector>

class PrimitiveArray {

public:

	std::vector<Primitive*> primitives;

	~PrimitiveArray() {
		for (auto a : primitives) {
			delete a;
		}
	}

};
