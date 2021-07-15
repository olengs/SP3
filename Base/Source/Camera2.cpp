#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	magnitude = Math::FAbs(target.z - pos.z);
}

void Camera2::Update(double dt, TRS player)
{
	static const float CAMERA_SPEED = 10.f;
	static const float ROTATE_SPEED = 135.f;

	Mtx44 rotation;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	Vector3 actualup = Vector3(0, 1, 0);

	up = right.Cross(view).Normalized();
	float yaw = (float)(ROTATE_SPEED * dt);
	rotation.SetToRotation(player.rotateAngles.y, 0, 1, 0);
	target = player.translate + Vector3(0, 5, 0);
	position = player.translate + Vector3(magnitude * sin(Math::DegreeToRadian(-player.rotateAngles.y)),
		8, magnitude * -cos(Math::DegreeToRadian(player.rotateAngles.y)));
	view = (target - position).Normalized();

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}