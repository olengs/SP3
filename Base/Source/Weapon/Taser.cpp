#include "Taser.h"

Taser::Taser()
{
}

Taser::~Taser()
{
}

void Taser::Init()
{
	Weapon::Init();

	magCapacity = INT_MAX;
	maxMagCapacity = INT_MAX;
	totalMag = 1;
	maxTotalMag = 1;
	timeBetweenShots = 2.f;
	reloadElapsedTime = 0.0f;
	damage = 3;
	zoomdis = 45.f;
	reloadTime = 0.f;
	elapsedTime = 0.0;
	fire = true;
}
