#pragma once
#include "../Obj.h"
#include "../Enemy.h"
#include "../SoundController/SoundController.h"

class Projectile
{
private:
	Vector3 pos;
	double lifetime;
	float speed;
	bool active;
	bool render;
	int damage;

	CSoundController* cSoundController;
public:
	Vector3 dir;
	TRS trs;
	Vector3 up;
	float pitch;
	Projectile();
	virtual ~Projectile();

	void Init(const Vector3& position, const Vector3& dir, const double lifetime, const float speed, const bool status, int damage, const bool render = true);
	void Update(const double dt);
	void CheckCollision(Obj* obj, vector<Obj*>& entities);

	void SetStatus(const bool status);
	bool IsActive() const;
	Vector3 GetPos() const;

	bool IsRender();
	inline void isOutOfBounds(Obj* terrain) {
		if (trs.translate.x < -terrain->trs.Scale.x / 2 || trs.translate.x > terrain->trs.Scale.x / 2
			|| trs.translate.z < -terrain->trs.Scale.z / 2 || trs.translate.z > terrain->trs.Scale.z / 2)
		{
			this->active = false;
		}
	}
};