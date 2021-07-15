#pragma once
#include <TRS.h>
#include "Mesh.h"
#include "Vertex.h"
struct Particle
{
	enum Particle_type {
		P_NONE = 0,
		P_ONE,
		P_RAIN,
		P_FOUNTAIN,
		P_TOTAL
	};

	Particle_type type;
	TRS transform;
	Vector3 vel;
	float DistFromCamSquare;
	float rotationSpeed;
	float DegreeToLight;
	float ftime;
	bool active;
	Mesh* mesh;
	float alpha;
	HSV hsv;

	Particle(Particle_type type = Particle_type::P_ONE);
	~Particle();
};
