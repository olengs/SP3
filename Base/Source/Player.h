#pragma once
#include <TRS.h>
#include "cornercoord.h"
#include "Mesh.h"
#include "LoadHmap.h"
#include "Camera3.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Obj.h"
#include "Application.h"
#include "Weapon/Weapon.h"
#include "Jump.h"
#include "SoundController/SoundController.h"

class Player : public Obj
{
private:
	const float CrouchingScaleRatio = 2.f;
	const float ProningScaleRatio = 6.f;
	Vector3 vec3Right;
	Vector3 vec3Target;
	Vector3 vec3Up;
	Vector3 vec3WorldUp;

	Vector3 InitialPos;
	Vector3 InitialScale;

	Vector3 LastStepPos;

	float fYaw;
	float fPitch;
	float zoom;
	float pMovementSpeed;
	bool isSprinting;
	bool isCrouching;
	bool isProne;
	//bool isRolling;
	bool isScoping;
	bool isCollidingY;

	Obj* terrain;
	Camera3* camera;
	std::vector<unsigned char> heightMap;

	Jump jump;
	CSoundController* cSoundController;
	Weapon* primaryWeapon;
	Weapon* secondaryWeapon;
	Weapon* tertiaryWeapon;
	Weapon* meleeWeapon;
	int currentWeapon;
	bool collidingWithPickUp;
public:
	Player();
	~Player();

	enum Player_Movement 
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		STATIONARY,
		NUM_MOVEMENTS
	};

	void SetSprint(bool sprint);
	void SetCrouch(bool crouch);
	void SetProne(bool prone);
	void SetScope(bool scope);
	void SetCollidingY(bool colliding);

	void SetCamera(Camera3* cam);
	void SetHeightMap(std::vector<unsigned char>& heightMap);
	void SetTerrain(Obj* ter);

	bool IsSprint(void);
	bool IsCrouch(void);
	bool IsProne(void);
	bool IsScope(void);
	bool IsCollidingY(void);
	float GetZoom();

	void ProcessMovement(const Player_Movement direction, const float fDeltaTime);
	void UpdatePlayerVectors(Camera3* camera, const float dt);
	void UpdatePlayerState();
	void SetPlayerToFloor();
	void SetPlayerToPoint(float y);
	void BindPlayerToBounds();
	Vector3& getvec3Target(void);
	void setvec3Target(Vector3& target);

	void SetToJump(void);
	void UpdateJumpFall(const double dElaspedTime);
	void Constraint(float minY);
	void UpdateCollision(float dt, vector<Obj*>& entities, Vector3& ResPoint, Vector3& ResTarget);
	float getPitch(void);

	float initialY;
	Vector3 GetDir() const;
	Vector3 GetRight();

	void SetWeapon(const int slot, Weapon* weapon);
	void SetCurrentWeapon(const int slot);
	Projectile* Shoot();
	Weapon* GetWeapon() const;
	Jump& getjump();

	bool IsCollidingWithPickUp();
};