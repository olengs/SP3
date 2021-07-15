#include "Pistol.h"

Pistol::Pistol()
{
}

Pistol::~Pistol()
{
}

void Pistol::Init()
{
	Weapon::Init();

	magCapacity = 5;
	maxMagCapacity = 5;
	totalMag = 8;
	maxTotalMag = 8;
	damage = 1;
	zoomdis = 35.f;
	timeBetweenShots = 0.3;
	reloadElapsedTime = 0.0f;
	reloadTime = 0.5f;
	elapsedTime = 0.0;
	fire = true;
}