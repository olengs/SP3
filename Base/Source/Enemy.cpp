#include "Enemy.h"

void Enemy::UpdateMovement(float dt)
{
	if (isStunned) {
		stunDuration -= dt;
		if (stunDuration < 0) {
			stunDuration = 5;
			isStunned = false;
		}
	}
	else {
		vel.y = 0;
		rollbackTRS = trs;

		try {
			trs.translate += vel.Normalize() * dt * moveSpeed;
		}
		catch (DivideByZero e) {

		}

		if (enemyState == ENEMY_STATE::CHASE) {
			chaseTime -= dt;
			if (chaseTime < 0) {
				chaseTime = 5;
				enemyState = ENEMY_STATE::IDLE;
				cSoundController->UpdateSoundPos(106, this->trs.translate);
				cSoundController->PlaySoundByID(106);
			}
		}
	}
}

void Enemy::SetPatrol(Vector3 patrolDir, Vector3 patrolStartPoint, float patrolCycleDuration)
{
	this->patrolDir = patrolDir;
	this->patrolCycleDuration = patrolCycleDuration;
	this->patrolStartPoint = patrolStartPoint;
	pBounceTime = patrolCycleDuration;
	enemyState = ENEMY_STATE::PATROL;
}

void Enemy::Stun(float stunDuration)
{
	this->stunDuration = stunDuration;
	isStunned = true;
}

void Enemy::Update(float dt)
{
	if (enemyState == ENEMY_STATE::PATROL) {
		pBounceTime -= dt;
		if (pBounceTime < 5) {
			patrolDir = -patrolDir;
			pBounceTime = patrolCycleDuration;
		}
	}
	if (isColliding) {
		collideDuration += dt;
		if (enemyState != ENEMY_STATE::PATROL && collideDuration > 0.5f) {
			vel.Set(cos(Math::DegreeToRadian(90)) * vel.x - sin(Math::DegreeToRadian(90)) * vel.z,
				vel.y,
				sin(Math::DegreeToRadian(90))* vel.x + cos(Math::DegreeToRadian(90)) * vel.z);
			collideDuration = 0;
		}
		trs = rollbackTRS;
	}
	UpdateMovement(dt);
}

Enemy::Enemy()
{
	mesh = nullptr;
	type = Obj::O_OTHER;
	toDestroy = 0;
	enemyState = ENEMY_STATE::IDLE;
	enemyType = ENEMY_TYPE::OTHER;
	this->cSoundController = CSoundController::GetInstance();
}

Enemy::Enemy(Obj* b)
{
	this->trs = b->trs;
	this->dimensions = b->dimensions;
	this->mesh = new Mesh(b->mesh, "copymesh");
	this->initialFront = b->initialFront;
	this->type = b->type;
	this->enemyType = ENEMY_TYPE::OTHER;
	this->enemyState = ENEMY_STATE::IDLE;
	this->cSoundController = CSoundController::GetInstance();
	toDestroy = 0;
}

Enemy::Enemy(Enemy* enemy)
{
	this->trs = enemy->trs;
	this->dimensions = enemy->dimensions;
	this->mesh = new Mesh(enemy->mesh, "copymesh");
	this->initialFront = enemy->initialFront;
	this->type = enemy->type;
	this->initialFront = enemy->initialFront;
	this->enemyState = enemy->enemyState;
	this->enemyType = enemy->enemyType;
	this->vel = enemy->vel;
	this->cSoundController = CSoundController::GetInstance();
	toDestroy = 0;
}
