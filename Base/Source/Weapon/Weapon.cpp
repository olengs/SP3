#include "Weapon.h"

Weapon::Weapon() :
	type(TYPE::SNIPER),
	magCapacity(10),
	maxMagCapacity(10),
	totalMag(3),
	damage(1),
	zoomdis(45.f),
	maxTotalMag(3),
	timeBetweenShots(1.0),
	elapsedTime(0.0),
	reloadTime(2.f),
	isReloading(false),
	fire(true),
	dir(0, 0, -1),
	cSoundController(NULL)
{
}

Weapon::~Weapon()
{
	if (cSoundController)
	{
		cSoundController = NULL;
	}
}

void Weapon::Init()
{
	magCapacity = 1;
	maxMagCapacity = 1;
	totalMag = 8;
	maxTotalMag = 8;
	reloadTime = 5.f;
	reloadElapsedTime = 0.0f;
	timeBetweenShots = 0.5;
	damage = 1;
	zoomdis = 45.f;
	elapsedTime = 0.0;
	fire = true;
	isReloading = false;

	cSoundController = CSoundController::GetInstance();
}

void Weapon::Update(const double dt)
{
	// If the weapon can fire, then don't check further
	if (fire)
		return;


//	std::cout << magCapacity << " / " << maxMagCapacity << "  -  " << totalMag << std::endl;

	if (isReloading == true)
	{
		reloadElapsedTime += dt;
		//std::cout << reloadElapsedTime << std::endl;
		//check if reload time has passed
		if (reloadElapsedTime > reloadTime)
		{
			--totalMag;
			magCapacity = maxMagCapacity;

			reloadElapsedTime = 0.0;
			isReloading = false;
			// Sounds for reloading
			switch (this->GetType())
			{
			case Weapon::SNIPER:
				cSoundController->PlaySoundByID(13);
				break;
			case Weapon::SHOTGUN:
				cSoundController->PlaySoundByID(12);
				break;
			case Weapon::PISTOL:
				cSoundController->PlaySoundByID(11);
				break;
			case Weapon::TASER:
				break;
			}
		}
	}
	else 
	{
		elapsedTime += dt;
		if (elapsedTime > timeBetweenShots)
		{
			fire = true;
			elapsedTime = 0.0;
		}
	}
}

Projectile* Weapon::Discharge(float fPitch, const Vector3& position, const Vector3& dir, const Vector3& up)
{
	if (fire)
	{
		if (magCapacity > 0)
		{
			Projectile* projectile = new Projectile;
			projectile->up = up;
			projectile->pitch = fPitch;
			switch (this->GetType())
			{
			case Weapon::SNIPER:
				projectile->Init(position + dir * 0.75f, dir, 4.0, 40.f, true, damage);
				cSoundController->PlaySoundByID(1);
				break;
			case Weapon::SHOTGUN:
				projectile->Init(position + dir * 0.75f, dir, 4.0, 40.f, true, damage);
				cSoundController->PlaySoundByID(3);
				break;
			case Weapon::PISTOL:
				projectile->Init(position + dir * 0.75f, dir, 4.0, 40.f, true, damage);
				cSoundController->PlaySoundByID(2);
				break;
			case Weapon::TASER:
				projectile->Init(position + dir, dir, 1.f, 0.f, true, damage, false);
				cSoundController->PlaySoundByID(10);
				break;
			}

			// Lock the weapon after this discharge
			fire = false;
			// Reduce the ammo by 1
			--magCapacity;

			return projectile;
		}
		else
		{
			// Sounds for empty clip
			switch (this->GetType())
			{
			case Weapon::SNIPER:
				cSoundController->PlaySoundByID(7);
				break;
			case Weapon::SHOTGUN:
				cSoundController->PlaySoundByID(9);
				break;
			case Weapon::PISTOL:
				cSoundController->PlaySoundByID(8);
				break;
			case Weapon::TASER:
				cSoundController->PlaySoundByID(8);
				break;
			}
		}
	}

	return nullptr;
}

void Weapon::Reload()
{
	//check if player has extra mags
	if (totalMag > 0)
	{
		//check if player current mag is lesser than max rounds in a mag
		if (magCapacity < maxMagCapacity)
		{
			isReloading = true;
			fire = false;
		}
	}
}

void Weapon::AddMag(const int newMag)
{
	if (totalMag + newMag > maxTotalMag)
		totalMag = maxTotalMag;
	else
		totalMag += newMag;
}

void Weapon::SetType(enum TYPE type)
{
	this->type = type;
}

void Weapon::SetMagCapacity(const int magCapacity)
{
	this->magCapacity = magCapacity;
}

void Weapon::SetMaxMagCapacity(const int maxMagCapacity)
{
	this->maxMagCapacity = maxMagCapacity;
}

void Weapon::SetTotalMag(const int totalMag)
{
	this->totalMag = totalMag;
}

void Weapon::SetMaxTotalMag(const int maxTotalMag)
{
	this->maxTotalMag = maxTotalMag;
}

void Weapon::SetTimeBetweenShots(const int timeBetweenShots)
{
	this->timeBetweenShots = timeBetweenShots;
}

void Weapon::SetFiringStatus(const bool fire)
{
	this->fire = fire;
}

void Weapon::SetDir(const Vector3& dir)
{
	this->dir = dir;
}

void Weapon::SetReloadTime(const double dReloadtime)
{
	this->reloadTime = dReloadtime;
}

int Weapon::GetType() const
{
	return type;
}

int Weapon::GetMagCapacity() const
{
	return magCapacity;
}

int Weapon::GetMaxMagCapacity() const
{
	return maxMagCapacity;
}

int Weapon::GetTotalMag() const
{
	return totalMag;
}

int Weapon::GetMaxTotalMag() const
{
	return maxTotalMag;
}

float Weapon::GetZoom() const
{
	return zoomdis;
}

double Weapon::GetTimeBetweenShots() const
{
	return timeBetweenShots;
}

bool Weapon::CanFire() const
{
	return fire;
}

Vector3 Weapon::GetDir() const
{
	return dir;
}

double Weapon::GetReloadTime(void) const
{
	return reloadTime;
}
