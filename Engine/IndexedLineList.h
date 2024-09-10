#pragma once

#include <vector>
#include "Vec3.h"

struct IndexedLineList
{
	std::vector<Vector3> vertices;
	std::vector<size_t> indices;
};