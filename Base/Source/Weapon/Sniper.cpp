#include "Sniper.h"

Sniper::Sniper()
{
}

Sniper::~Sniper()
{
}

void Sniper::Init()
{
	Weapon::Init();

	magCapacity = 10;
	maxMagCapacity = 10;
	totalMag = 3;
	maxTotalMag = 3;
	timeBetweenShots = 1.5;
	reloadElapsedTime = 0.0f;
	damage = 5;
	zoomdis = 10.f;
	reloadTime = 2.f;
	elapsedTime = 0.0;
	fire = true;
}