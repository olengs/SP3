#include "Player.h"
#include "Collision_detector.h"

Player::Player()
{
	//pMovementSpeed = 100.0f;
	pMovementSpeed = 20.0f;
	isCrouching = false;
	isProne = false;
	isScoping = false;
	isSprinting = false;
	camera = NULL;
	terrain = NULL;
	//heightMap = 0;
	fYaw = -0.f;
	fPitch = 0.f;
	trs.translate = Vector3(0, 0, 450.f);
	//trs.translate = Vector3(0, 0, 0);
	initialFront = (Vector3(0.0f, 0.0f, -1.0f));
	vec3Front = initialFront;
	vec3Up = Vector3(0, 1, 0);
	vec3Right = Vector3(1.0f, 1.0f, 0.0f);
	vec3Target = Vector3(0, 0, 80);
	vec3WorldUp = (Vector3(0.0f, 1.0f, 0.0f));
	InitialScale = Vector3(0.25, 0.75f, 0.25);
	this->trs.Scale = InitialScale;
	InitialPos = Vector3(0, 0, 0);
	LastStepPos = InitialPos;
	zoom = 45.f;
	jump.Init();
	jump.SetStatus(Jump::STATUS::FALL);
	primaryWeapon = NULL;
	secondaryWeapon = NULL;
	tertiaryWeapon = NULL;
	meleeWeapon = NULL;
	currentWeapon = 0;
	cSoundController = CSoundController::GetInstance();
}

Player::~Player()
{
	if (cSoundController)
	{
		cSoundController = NULL;
	}
}

void Player::SetSprint(bool sprint)
{
	isSprinting = sprint;
}

void Player::SetCrouch(bool crouch)
{
	isCrouching = crouch;
}

void Player::SetProne(bool prone)
{
	isProne = prone;
}

void Player::SetScope(bool scope)
{
	isScoping = scope;
}

void Player::SetCollidingY(bool colliding)
{
	this->isCollidingY = colliding;
}

void Player::SetCamera(Camera3* cam)
{
	this->camera = cam;
}

void Player::SetHeightMap(std::vector<unsigned char>& heightMap)
{
	this->heightMap = heightMap;
}

void Player::SetTerrain(Obj* ter)
{
	this->terrain = ter;
}

bool Player::IsSprint(void)
{
	if (this->isSprinting)
		return true;
	return false;
}

bool Player::IsCrouch(void)
{
	if (this->isCrouching)
		return true;
	return false;
}

bool Player::IsProne(void)
{
	if (this->isProne)
		return true;
	return false;
}

bool Player::IsScope(void)
{
	if (this->isScoping)
		return true;
	return false;
}

bool Player::IsCollidingY(void)
{
	return isCollidingY;
}

float Player::GetZoom()
{
	return zoom;
}

void Player::ProcessMovement(const Player_Movement direction, const float fDeltaTime)
{
	//if sprinting set speed to 4
	if (this->isSprinting)
	{
		this->pMovementSpeed = 30.f;
	}
	else if (this->isProne)
	{
		this->pMovementSpeed = 5.f;
	}
	else if (this->isCrouching)
	{
		this->pMovementSpeed = 10.f;
	}
	else if (this->isScoping)
	{
		this->pMovementSpeed = 10.f;
		if (isProne)
			this->pMovementSpeed = 5.f;
	}
	// normal speed
	else if (!this->isSprinting && !this->isCrouching && !this->isProne &&/* !isRolling && */!this->isScoping)
	{
		this->pMovementSpeed = 20.f;
	}
	float velocity = pMovementSpeed * fDeltaTime;
	//ONLY CAN SPRINT FORWARDS
	if (direction == FORWARD)
	{
		this->trs.translate += vec3Front * velocity;
		this->vec3Target += vec3Front * velocity;
		//std::cout << velocity << std::endl;
	}
	if (direction == BACKWARD)
	{
		if (isSprinting)
		{
			velocity = 20.f * fDeltaTime;
		}
		this->trs.translate -= vec3Front * velocity;
		this->vec3Target -= vec3Front * velocity;
		//cout << velocity << endl;
	}
	if (direction == LEFT)
	{
		// cap velocity so cant sprint sideways
		if (isSprinting)
		{
			velocity = 20.f * fDeltaTime;
		}
		Vector3 vec3Right = vec3Front.Cross(vec3WorldUp).Normalize();
		this->trs.translate -= vec3Right * velocity;
		this->vec3Target -= vec3Right * velocity;
		//std::cout << vec3Front << "              " << vec3Right << std::endl;
		//std::cout << velocity << std::endl;
	}
	if (direction == RIGHT)
	{
		if (isSprinting)
		{
			velocity = 20.f * fDeltaTime;
		}
		Vector3 vec3Right = vec3Front.Cross(vec3WorldUp).Normalize();
		this->trs.translate += vec3Right * velocity;
		this->vec3Target += vec3Right * velocity;
		//cout << velocity << endl;
	}

	if ((this->trs.translate - this->LastStepPos).Length() > 10)
	{
		if (jump.GetStatus() == Jump::STATUS::IDLE)
		{
			if(!isCrouching && !isProne)
				cSoundController->PlaySoundByID(16);
			this->LastStepPos = this->trs.translate;
		}
	}
}

void Player::UpdatePlayerVectors(Camera3* camera, const float dt)
{
//	std::cout << vec3Target << "\n";
	{
		Vector3 view = (this->vec3Target - this->trs.translate).Normalized();
		float yaw = (float)(-50.f * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		this->vec3Target = this->trs.translate + view;
		Vector3 right = view.Cross(this->vec3Up);
		right.y = 0;
		right.Normalize();
		this->vec3Up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-50.f * Application::camera_pitch * (float)dt);
		fPitch += pitch;
		Vector3 view = (this->vec3Target - this->trs.translate).Normalized();
		Vector3 right = view.Cross(this->vec3Up);
		right.y = 0;
		right.Normalize();
		this->vec3Up = right.Cross(view).Normalized();

		if (fPitch < 80.0f && fPitch > -80.0f)
		{
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
		}

		if (fPitch > 80.0f)
		{
			fPitch = 80.0f;
		}
		if (fPitch < -80.0f)
		{
			fPitch = -80.0f;
		}
    
		this->vec3Target = this->trs.translate + view;
	}

	Vector3 front;
	front.x = this->vec3Target.x - this->trs.translate.x;
	front.y = 0.f;
	front.z = this->vec3Target.z - this->trs.translate.z;
	vec3Front = front.Normalize();

	camera->target = this->vec3Target;
	camera->position = this->trs.translate;
	camera->up = this->vec3Up;

	zoom = 45.f;
	if (IsScope())
	{
		//get zoom of curr weapon
		zoom = GetWeapon()->GetZoom();
	}

	GetWeapon()->SetDir((camera->target - camera->position).Normalized());

	//std::cout << GetWeapon()->GetDir() << "\n";
}

void Player::UpdatePlayerState()
{
	float tempcamy = camera->position.y;

	float tempplayscaley = this->InitialScale.y / CrouchingScaleRatio;
	float tempplayscaley2 = this->InitialScale.y / ProningScaleRatio;

	if (isCrouching == true)
	{
		Vector3 view = (camera->target - camera->position);
		this->trs.Scale.y = tempplayscaley;
		camera->position = this->trs.translate;
		camera->target = view + camera->position;
	}
	else if (isProne == true)
	{
		Vector3 view = (camera->target - camera->position);
		this->trs.Scale.y = tempplayscaley2;
		camera->position = this->trs.translate;
		camera->target = view + camera->position;
	}
	else {
		Vector3 view = (camera->target - camera->position);
		this->trs.Scale.y = InitialScale.y;
		camera->position = this->trs.translate;
		camera->position.y = this->initialY;
		camera->target = view + camera->position;
	}
}

void Player::SetPlayerToFloor()
{
	Vector3 view = (this->vec3Target - this->trs.translate).Normalize();
	this->trs.translate.y = ReadHeightMap(heightMap, this->trs, terrain->trs)
		- this->dimensions.getminY() * this->trs.Scale.y;
	this->vec3Target = view + this->trs.translate;
}

void Player::SetPlayerToPoint(float y)
{
	Vector3 olddir = (this->vec3Target - this->trs.translate).Normalized();
	this->trs.translate.y = y;
	this->vec3Target = this->trs.translate + olddir;
}

void Player::BindPlayerToBounds()
{
	Vector3 view = (this->vec3Target - this->trs.translate).Normalize();
	//out of bounds -> push back
	if (trs.translate.x < -terrain->trs.Scale.x / 2) {
		trs.translate.x = -terrain->trs.Scale.x / 2;
	}
	else if (trs.translate.x > terrain->trs.Scale.x / 2) {
		trs.translate.x = terrain->trs.Scale.x / 2;
	}
	else if (trs.translate.z < -terrain->trs.Scale.z / 2) {
		trs.translate.z = -terrain->trs.Scale.z / 2;
	}
	else if (trs.translate.z > terrain->trs.Scale.z / 2) {
		trs.translate.z = terrain->trs.Scale.z / 2;
	}

	this->vec3Target = view + this->trs.translate;
}

Vector3& Player::getvec3Target(void)
{
	// TODO: insert return statement here
	return this->vec3Target;
}

void Player::setvec3Target(Vector3& target)
{
	this->vec3Target = target;
}

void Player::SetToJump(void)
{
	if (jump.GetStatus() == Jump::STATUS::IDLE)
	{
		jump.SetStatus(Jump::STATUS::JUMP);
		jump.SetInitialVelocity(Vector3(0.0f, 40.0f, 0.0f));
	}
}

void Player::UpdateJumpFall(const double dElaspedTime)
{
	if (jump.GetStatus() == Jump::STATUS::JUMP)
	{
		//Update the elasped time
		jump.AddElapsedTime((float)dElaspedTime);
		//update final velocity and displacement
		jump.Update();
		//Get the displacement and update the player position
		Vector3 view = (this->vec3Target - this->trs.translate).Normalize();
		trs.translate = trs.translate + jump.GetDisplacement();
		BindPlayerToBounds();
		this->vec3Target = view + this->trs.translate;

		//If the player is still jumping and initial velocity has reached zero or below zero
		// then it has reached the peak of its jump
		if ((jump.GetStatus() == Jump::STATUS::JUMP) && (jump.GetInitialVelocity().y <= 0.0f))
		{
			// Set status to fall
			jump.SetStatus(Jump::STATUS::FALL);
		}
	}
	else if (jump.GetStatus() == Jump::STATUS::FALL)
	{
		// Update the elasped time to the physics engine
		jump.AddElapsedTime((float)dElaspedTime);
		//Call the physics engine update method to calculate the final velocity and displacement
		jump.Update();
		//Get the displacement from the physics engine and update the player position
		Vector3 view = (this->vec3Target - this->trs.translate).Normalize();
		trs.translate = trs.translate + jump.GetDisplacement();
		BindPlayerToBounds();
		//Constrain the player's position to the groundmap
		Vector3 vec3CheckPosition = ReadHeightMap(heightMap, this->trs, terrain->trs)
			- this->dimensions.getminY() * this->trs.Scale.y;

		// Set the physics to idle status
		if (vec3CheckPosition.y - this->dimensions.getminY() > trs.translate.y)
		{
			trs.translate.y = vec3CheckPosition.y;
			jump.SetStatus(Jump::STATUS::IDLE);
			cSoundController->PlaySoundByID(17);
		}
		this->vec3Target = view + this->trs.translate;
	}

}

void Player::Constraint(float minY)
{
	if (jump.GetStatus() == Jump::STATUS::IDLE)
	{
		// If the player is not jumping nor falling, then we snap his position to the cGroundMap
		if (minY == 0)
			SetPlayerToFloor();
		else
			SetPlayerToPoint(minY);
		BindPlayerToBounds();
	}
	else
	{
		// If the player is jumping or falling, then we only snap his position if the player's y-coordinate is below the cGroundMap 
		Vector3 view = (this->vec3Target - this->trs.translate).Normalize();
		Vector3 tempVec3Position = ReadHeightMap(heightMap, this->trs, terrain->trs)
			- this->dimensions.getminY() * this->trs.Scale.y;
		/*trs.translate.x = tempVec3Position.x;
		trs.translate.z = tempVec3Position.z;*/
		if (trs.translate.y < tempVec3Position.y)
			trs.translate.y = tempVec3Position.y;
		this->vec3Target = view + this->trs.translate;
	}
}

void Player::UpdateCollision(float dt, vector<Obj*>& entities, Vector3& ResPoint, Vector3& ResTarget)
{
	float tempcamy = camera->position.y;
	float tempplayscaley = this->InitialScale.y / CrouchingScaleRatio;
	float tempplayscaley2 = this->InitialScale.y / ProningScaleRatio;
	Vector3 view = (ResTarget - ResPoint).Normalize();
	if (isCrouching)
	{
		this->trs.Scale.y = tempplayscaley;
	}
	else if (isProne)
	{
		this->trs.Scale.y = tempplayscaley2;

	}
	else {
		this->trs.Scale.y = InitialScale.y;
	}
	BindPlayerToBounds();
	int size = entities.size();
	Vector3 vec3CheckPosition = ReadHeightMap(heightMap, this->trs, terrain->trs)
		- this->dimensions.getminY() * this->trs.Scale.y;
	float lowestY = vec3CheckPosition.y;

	if (jump.GetStatus() == Jump::JUMP)
	{
		jump.AddElapsedTime(dt);
		jump.Update();
		trs.translate = trs.translate + jump.GetDisplacement();
		BindPlayerToBounds();
		this->vec3Target = view + this->trs.translate;
		if ((jump.GetStatus() == Jump::STATUS::JUMP) && (jump.GetInitialVelocity().y <= 0.0f))
		{
			jump.SetStatus(Jump::STATUS::FALL);
		}
	}
	else if (jump.GetStatus() == Jump::FALL) {
		jump.AddElapsedTime(dt);
		jump.Update();
		trs.translate = trs.translate + jump.GetDisplacement();
	}

	collidingWithPickUp = false;
	for (int i = 0; i < size; ++i) {
		if (jump.GetStatus() == Jump::JUMP) {
			if (collision_detection::collision_detector(this, entities[i])) {
				if (this->trs.translate.y + this->dimensions.getmaxY() * this->trs.Scale.y - 0.4f
					< entities[i]->trs.translate.y + entities[i]->dimensions.getminY() * entities[i]->trs.Scale.y) {
					this->trs.translate.y = entities[i]->trs.translate.y + entities[i]->dimensions.getminY()
						* entities[i]->trs.Scale.y - this->dimensions.getmaxY() * this->trs.Scale.y - 0.1f;
					this->setvec3Target(ResTarget - ResPoint + this->trs.translate);
					jump.SetStatus(Jump::FALL);
					Vector3 temp = jump.GetInitialVelocity();
					temp.y = 0;
					jump.SetInitialVelocity(temp);
				}
				else {
					this->trs.translate = ResPoint;
					this->vec3Target = ResTarget;
					trs.translate = trs.translate + jump.GetDisplacement();
				}
			}
		}
		else if (jump.GetStatus() == Jump::FALL) {
			if (collision_detection::collision_detector(this, entities[i])) {
				if (this->trs.translate.y >
					entities[i]->trs.translate.y + entities[i]->dimensions.getmaxY() * entities[i]->trs.Scale.y) {
					lowestY = Math::Max(lowestY, entities[i]->trs.translate.y + entities[i]->dimensions.getmaxY() * entities[i]->trs.Scale.y - 0.1f);
					this->trs.translate -= jump.GetDisplacement();
					jump.SetStatus(Jump::IDLE);
				}
				else {
					this->trs.translate = ResPoint;
					this->vec3Target = ResTarget;
					trs.translate = trs.translate + jump.GetDisplacement();
				}
			}
		}
		else if (jump.GetStatus() == Jump::IDLE) {
			if ((this->trs.translate.y + this->dimensions.getminY() * this->trs.Scale.y - 0.2f
				< entities[i]->trs.translate.y + entities[i]->dimensions.getmaxY() * entities[i]->trs.Scale.y)
				&& collision_detection::collision_detector(this, entities[i]))
			{
				if (entities[i]->type == Obj::O_ITEMS)
				{
					collidingWithPickUp = true;
					if (Application::IsKeyPressed('E'))
					{
						cSoundController->PlaySoundByID(18);
						this->GetWeapon()->SetTotalMag(this->GetWeapon()->GetMaxTotalMag()); // Refill ammo
						entities[i]->toDestroy = true;
						entities.erase(entities.begin() + i);
						--size;
					}
				}
				else
				{
					this->trs.translate = ResPoint;
					this->vec3Target = ResTarget;
				}
			}
		}
	}
	if (this->trs.translate.y + this->dimensions.getminY() * this->trs.Scale.y - 1.f > lowestY && jump.GetStatus() == Jump::IDLE) {
		jump.SetStatus(Jump::STATUS::FALL);
	}
	if (this->trs.translate.y + this->dimensions.getminY() * this->trs.Scale.y < lowestY && jump.GetStatus() == Jump::FALL)
	{
		trs.translate.y = lowestY - this->dimensions.getminY() * this->trs.Scale.y;
		jump.SetStatus(Jump::STATUS::IDLE);
		cSoundController->PlaySoundByID(17);
		//camera->target = view + this->trs.translate;
	}
	if (!isCrouching && !isProne && jump.GetStatus() == Jump::IDLE) {
		trs.translate.y = lowestY - this->dimensions.getminY() * this->trs.Scale.y;
	}
	this->vec3Target = view + this->trs.translate;
	camera->position = this->trs.translate;
	camera->target = view + camera->position;
}

float Player::getPitch(void)
{
	return fPitch;
}

Vector3 Player::GetDir() const
{
	return (this->vec3Target - this->trs.translate).Normalize();
}

Vector3 Player::GetRight()
{
	return this->vec3Front.Cross(vec3WorldUp);
}

void Player::SetWeapon(const int slot, Weapon* weapon)
{
	switch (slot)
	{
	case Weapon::SNIPER:
		primaryWeapon = weapon;
		break;
	case Weapon::SHOTGUN:
		secondaryWeapon = weapon;
		break;
	case Weapon::PISTOL:
		tertiaryWeapon = weapon;
		break;
	case Weapon::TASER:
		meleeWeapon = weapon;
		break;
	}
}

void Player::SetCurrentWeapon(const int slot)
{
	currentWeapon = slot;
}

Projectile* Player::Shoot()
{
	if (currentWeapon == Weapon::SNIPER && primaryWeapon)	// Checking if weapon exist is probably not needed
		return primaryWeapon->Discharge(this->fPitch, trs.translate, (this->vec3Target - this->trs.translate).Normalize(),this->vec3Up);
	else if(currentWeapon == Weapon::SHOTGUN && secondaryWeapon)
		return secondaryWeapon->Discharge(this->fPitch, trs.translate, (this->vec3Target - this->trs.translate).Normalize(), this->vec3Up);
	else if (currentWeapon == Weapon::PISTOL && tertiaryWeapon)
		return tertiaryWeapon->Discharge(this->fPitch, trs.translate, (this->vec3Target - this->trs.translate).Normalize(), this->vec3Up);
	else if (currentWeapon == Weapon::TASER && meleeWeapon)
		return meleeWeapon->Discharge(this->fPitch, trs.translate, (this->vec3Target - this->trs.translate).Normalize(), this->vec3Up);

	return nullptr;
}

Weapon* Player::GetWeapon() const
{
	switch (currentWeapon)
	{
	case Weapon::SNIPER:
		return primaryWeapon;
	case Weapon::SHOTGUN:
		return secondaryWeapon;
	case Weapon::PISTOL:
		return tertiaryWeapon;
	case Weapon::TASER:
		return meleeWeapon;
	}

	return nullptr;
}

Jump& Player::getjump()
{
	// TODO: insert return statement here
	return this->jump;
}

bool Player::IsCollidingWithPickUp()
{
	if (collidingWithPickUp)
		return true;
	return false;
}
