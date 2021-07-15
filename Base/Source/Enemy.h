#ifndef ENEMY_H
#define ENEMY_H

#include <TRS.h>
#include "cornercoord.h"
#include "Mesh.h"
#include "LoadHmap.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Obj.h"
#include "Application.h"
#include "SoundController/SoundController.h"

class Enemy : public Obj
{
private:
	CSoundController* cSoundController;
public:
	enum ENEMY_TYPE {
		CITIZEN,
		POLICE,
		OTHER
	};
	enum ENEMY_STATE
	{
		IDLE,
		PATROL,
		REPORT,
		CHASE
	};
	ENEMY_TYPE enemyType;
	ENEMY_STATE enemyState;

	float chaseTime = 2;

	float patrolCycleDuration = 5;
	float pBounceTime = patrolCycleDuration;
	Vector3 patrolDir;
	Vector3 patrolStartPoint;

	Vector3 vel;

	float visionRange = 75.f;
	float visionRadius = Math::PI / 4;

	float moveSpeed = 10;

	float stunDuration = 5;
	bool isStunned = false;

	bool isColliding = false;
	float collideDuration = 0.f;

	TRS rollbackTRS = trs;

	void UpdateMovement(float dt);
	void SetPatrol(Vector3 patrolDir, Vector3 patrolStartPoint, float patrolCycleDuration = 5);
	void Stun(float stunDuration);
	void Update(float dt);

	Enemy();
	Enemy(Obj* b);
	Enemy(Enemy* enemy);
};
#endif // !ENEMY_H
