#include "Projectile.h"
#include "../Collision_detector.h"
Projectile::Projectile() :
	pos(0, 0, 0),
	dir(0, 0, 0),
	lifetime(1.0),
	speed(1.f),
	active(false),
	cSoundController(NULL)
{
}

Projectile::~Projectile()
{
	if (cSoundController)
	{
		cSoundController = NULL;
	}
}

void Projectile::Init(const Vector3& position, const Vector3& dir, const double lifetime, const float speed, const bool status, int damage, const bool render)
{
	this->pos = position;
	this->dir = dir;
	this->lifetime = lifetime;
	this->speed = speed;
	this->active = status;
	this->damage = damage;
	this->render = render;

	cSoundController = CSoundController::GetInstance();
}

void Projectile::Update(const double dt)
{
	if (!active)
		return;

	lifetime -= dt;
	if (lifetime < 0.0)
	{
		SetStatus(false);
		return;
	}

	pos = pos + dir * static_cast<float>(dt) * speed;
}

void Projectile::CheckCollision(Obj* obj, vector<Obj*>& entities)
{
	int size = entities.size();
	for (int i = 0; i < size; ++i) {
		TRS trs;
		trs.translate = this->pos;
		trs.Scale = obj->trs.Scale;
		if (collision_detection::collision_detector(trs, obj->dimensions, entities[i]->trs, entities[i]->dimensions)) {
			this->active = false;
			if (entities[i]->type == Obj::O_ENEMY && ((Enemy*)entities[i])->enemyType != Enemy::ENEMY_TYPE::POLICE) {
				((Enemy*)entities[i])->Stun(damage);
				cSoundController->UpdateSoundPos(104, entities[i]->trs.translate);
				cSoundController->PlaySoundByID(104);
			}
		}
		
	}

}

void Projectile::SetStatus(const bool status)
{
	active = status;
}

bool Projectile::IsActive() const
{
	return active;
}

Vector3 Projectile::GetPos() const
{
	return pos;
}

bool Projectile::IsRender()
{
	return this->render;
}
