#pragma once
#include "Projectile.h"

#include "..\SoundController/SoundController.h"

class Weapon
{
protected:
	int type;
	int magCapacity;			// Number of bullets in a magazine
	int maxMagCapacity;			// Maximum number of bullets in a magazine
	int totalMag;				// Number of magazine player is carrying
	int maxTotalMag;			// Total number of magazine player can carry
	int damage;					// Damage of bullets
	float zoomdis;				// Zoom distance of weapon when scoped
	double timeBetweenShots;	// Time between each shot
	double elapsedTime;
	bool fire;					// Indicates if weapon can shoot
	bool isReloading;			// Indicates if weapon is reloading
	double reloadTime;			// The time needed to reload gun
	double reloadElapsedTime;	// The reload elapsed time 
	Vector3 dir;				// Direction the weapon is facing			
	CSoundController* cSoundController;
public:
	enum TYPE : short
	{
		SNIPER,
		SHOTGUN,
		PISTOL,
		TASER
	};

	Weapon();
	virtual ~Weapon();

	virtual void Init();
	virtual void Update(const double dt);
	virtual Projectile* Discharge(float fPitch, const Vector3& position, const Vector3& dir, const Vector3& up);
	virtual void Reload();
	virtual void AddMag(const int newMag); 

	virtual void SetType(enum TYPE type);
	virtual void SetMagCapacity(const int magCapacity);
	virtual void SetMaxMagCapacity(const int maxMagCapacity);
	virtual void SetTotalMag(const int totalMag);
	virtual void SetMaxTotalMag(const int maxTotalMag);
	virtual void SetTimeBetweenShots(const int timeBetweenShots);
	virtual void SetFiringStatus(const bool fire);
	virtual void SetDir(const Vector3& dir);
	virtual void SetReloadTime(const double dReloadtime);

	virtual int GetType() const;
	virtual int GetMagCapacity() const;
	virtual int GetMaxMagCapacity() const;
	virtual int GetTotalMag() const;
	virtual int GetMaxTotalMag() const;
	virtual float GetZoom() const;
	virtual double GetTimeBetweenShots() const;
	virtual bool CanFire() const;
	virtual Vector3 GetDir() const;
	virtual double GetReloadTime(void) const;
};