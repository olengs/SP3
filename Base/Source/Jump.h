
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

class Jump
{
public:
	enum STATUS
	{
		IDLE = 0,
		JUMP,
		FALL,
		NUM_STATUS
	};

	// Constructor
	Jump(void);

	// Destructor
	virtual ~Jump(void);

	// Init
	bool Init(void);

	// Set methods
	void SetInitialVelocity(const Vector3 v3InitialVelocity);	// Set Initial velocity
	void SetFinalVelocity(const Vector3 v3FinalVelocity);		// Set Final velocity
	void SetAcceleration(const Vector3 v3Acceleration);		// Set Acceleration
	void SetDisplacement(const Vector3 v3Displacement);		// Set Displacement
	void SetTotalElapsedTime(const float fTotalElapsedTime);	// Set Total Elapsed Time
	void SetElapsedTime(const float fElapsedTime);				// Set Elapsed Time
	void SetStatus(const STATUS sStatus);						// Set Status

	// Get methods
	Vector3 GetInitialVelocity(void) const;	// Get Initial velocity
	Vector3 GetFinalVelocity(void) const;		// Get Final velocity
	Vector3 GetAcceleration(void) const;		// Get Acceleration
	Vector3 GetDisplacement(void) const;		// Get Displacement
	float GetTotalElapsedTime(void) const;		// Get Total Elapsed Time
	STATUS GetStatus(void) const;				// Get Status

	// Update
	void Update(void);

	// Add elapsed time
	void AddElapsedTime(const float fElapsedTime);

protected:
	// Variables for SUVAT calculations
	Vector3 v3InitialVelocity;	// Initial velocity
	Vector3 v3FinalVelocity;		// Final velocity
	Vector3 v3Acceleration;		// Acceleration
	Vector3 v3Displacement;		// Displacement
	float fTotalElapsedTime;		// Total elapsed time since the motion was started
	float fElapsedTime;				// Elapsed time between the previous and current update

	const Vector3 v3Gravity = Vector3( 0.0f, -30.5f, 0.0f);		// Gravity constant

	STATUS sCurrentStatus;
};

