
#include "Jump.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
Jump::Jump(void)
	: v3InitialVelocity(Vector3(0.0f))
	, v3FinalVelocity(Vector3(0.0f))
	, v3Acceleration(Vector3(0.0f))
	, v3Displacement(Vector3(0.0f))
	, fTotalElapsedTime(0.0f)
	, fElapsedTime(0.0f)
	, sCurrentStatus(STATUS::IDLE)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
Jump::~Jump(void)
{
}

/**
@brief Init Initialise this instance
*/ 
bool Jump::Init(void)
{
	// Reset these variables
	v3InitialVelocity = Vector3(0.0f);
	v3FinalVelocity = Vector3(0.0f);
	v3Acceleration = Vector3(0.0f);		// Acceleration does not need to be reset here.
	v3Displacement = Vector3(0.0f);
	fTotalElapsedTime = 0.0f;
	fElapsedTime = 0.0f;
	return true;
}

// Set methods
// Set Initial velocity
void Jump::SetInitialVelocity(const Vector3 v3InitialVelocity)
{
	this->v3InitialVelocity = v3InitialVelocity;	// Initial velocity
}

// Set Final velocity
void Jump::SetFinalVelocity(const Vector3 v3FinalVelocity)
{
	this->v3FinalVelocity = v3FinalVelocity;		// Final velocity
}

// Set Acceleration
void Jump::SetAcceleration(const Vector3 v3Acceleration)
{
	this->v3Acceleration = v3Acceleration;		// Acceleration
}

// Set Displacement
void Jump::SetDisplacement(const Vector3 v3Displacement)
{
	this->v3Displacement = v3Displacement;		// Displacement
}

// Set Total Elapsed Time
void Jump::SetTotalElapsedTime(const float fTotalElapsedTime)
{
	this->fTotalElapsedTime = fTotalElapsedTime;	// Total Elapsed Time
}

// Set Elapsed Time
void Jump::SetElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;	// Total Elapsed Time
}

// Set Status
void Jump::SetStatus(const STATUS sStatus)
{
	// If there is a change in status, then reset to default values
	if (sCurrentStatus != sStatus)
	{
		// Reset to default values
		Init();

		// Store the new status
		sCurrentStatus = sStatus;
	}
}

// Get methods
// Get Initial velocity
Vector3 Jump::GetInitialVelocity(void) const
{
	return v3InitialVelocity;	// Initial velocity
}

// Get Final velocity
Vector3 Jump::GetFinalVelocity(void) const
{
	return v3FinalVelocity;		// Final velocity
}

// Get Acceleration
Vector3 Jump::GetAcceleration(void) const
{
	return v3Acceleration;		// Acceleration
}

// Get Displacement
Vector3 Jump::GetDisplacement(void) const
{
	return v3Displacement;		// Displacement
}

// Get Time
float Jump::GetTotalElapsedTime(void) const
{
	return fTotalElapsedTime;	// Total Elapsed Time
}

// Get Status
Jump::STATUS Jump::GetStatus(void) const
{
	return sCurrentStatus;
}

// Update
void Jump::Update(void)
{
	if (sCurrentStatus == JUMP)
	{
		// Calculate the final velocity
		v3FinalVelocity = v3InitialVelocity + (v3Acceleration + v3Gravity)* fElapsedTime;
		// Calculate the updated acceleration
		v3Acceleration.x = (v3FinalVelocity.x - v3InitialVelocity.x) / fElapsedTime;
		v3Acceleration.y = (v3FinalVelocity.y - v3InitialVelocity.y) / fElapsedTime;
		v3Acceleration.z = (v3FinalVelocity.z - v3InitialVelocity.z) / fElapsedTime;
		// Calculate the displacement
		v3Displacement = v3FinalVelocity * fElapsedTime - 0.5f * (v3Acceleration + v3Gravity) * fElapsedTime * fElapsedTime;
		// Update v3InitialVelocity
		v3InitialVelocity = v3FinalVelocity;

	}
	else if (sCurrentStatus == FALL)
	{
		// Calculate the final velocity
		v3FinalVelocity = v3InitialVelocity + v3Gravity * fElapsedTime;
		// Calculate the displacement
		v3Displacement = v3FinalVelocity * fElapsedTime - 0.5f * v3Gravity * fElapsedTime * fElapsedTime;
		// Update v3InitialVelocity
		v3InitialVelocity = v3FinalVelocity;
	}
	//std::cout << v3Displacement << std::endl;
}

// Add elapsed time
void Jump::AddElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;
	fTotalElapsedTime += fElapsedTime;
}

