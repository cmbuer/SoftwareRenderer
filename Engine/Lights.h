#pragma once
#include "Math3D.h"

struct DirectionalLight {
	Vector3 direction;
	Vector3 diffuseColor;
	Vector3 ambientColor;
};

struct PointLight {
	Vector3 position;
	Vector3 diffuseColor;
	Vector3 ambientColor;

	// attenuation coeffecients 
	// A = 1/(ad^2 + bd +c)
	float quadradicAttenuation;		// a
	float linearAttenuation;		// b
	float constantAttenuation;		// c
};

