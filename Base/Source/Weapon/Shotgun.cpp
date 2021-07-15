#include "Shotgun.h"

Shotgun::Shotgun()
{
}

Shotgun::~Shotgun()
{
}

void Shotgun::Init()
{
	Weapon::Init();

	magCapacity = 6;
	maxMagCapacity = 6;
	totalMag = 5;
	maxTotalMag = 5;
	damage = 3;
	zoomdis = 30.f;
	timeBetweenShots = 0.8;
	reloadElapsedTime = 0.0f;
	reloadTime = 1.5f;
	elapsedTime = 0.0;
	fire = true;
}
