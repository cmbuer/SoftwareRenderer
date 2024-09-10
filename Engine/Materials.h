#pragma once
#include "Math3D.h"

struct PhongMaterial {
	Vector3 specularReflection;
	Vector3 diffuseReflection;
	Vector3 ambientReflection;
	float shininess;
};