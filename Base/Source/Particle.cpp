#include "Particle.h"

Particle::Particle(Particle_type type)
	:type(type),
	active(true),
	DistFromCamSquare(0)
	, ftime(0)
	, DegreeToLight(0)
{
	transform.Scale.Set(1,1,1);
}

Particle::~Particle()
{
}
