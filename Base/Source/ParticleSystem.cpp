#include "ParticleSystem.h"

ParticleSystem::ParticleSystem():
	numParticles(0),
	mesh(nullptr),
	StartPos(Vector3(0)),
	StartSpeed(Vector3(0)),
	EndSpeed(Vector3(0)),
	StartScale(Vector3(1)),
	EndScale(Vector3(1)),
	startAlpha(1),
	endAlpha(1),
	ParticlesPerFrame(0),
	isGravityOn(0),
	radius(0),
	isadditive(0),
	ftime(0),
	isactive(1)
{
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void ParticleSystem::setStartPos(Vector3& pos)
{
	this->StartPos = pos;
}

void ParticleSystem::setStartPos(float a, float b, float c)
{
	this->StartPos = Vector3(a, b, c);
}

void ParticleSystem::setStartSpeed(Vector3& speed)
{
	this->StartSpeed = speed;
}

void ParticleSystem::setStartSpeed(float a, float b, float c)
{
	this->StartSpeed = Vector3(a, b, c);
}

void ParticleSystem::setEndSpeed(Vector3& speed)
{
	this->EndSpeed = speed;
}

void ParticleSystem::setEndSpeed(float a, float b, float c)
{
	this->EndSpeed = Vector3(a, b, c);
}

void ParticleSystem::setStartScale(Vector3& scale)
{
	this->StartScale = scale;
}

void ParticleSystem::setStartScale(float a, float b, float c)
{
	this->StartScale = Vector3(a, b, c);
}

void ParticleSystem::setEndScale(Vector3& scale)
{
	this->EndScale = scale;
}

void ParticleSystem::setEndScale(float a, float b, float c)
{
	this->EndScale = Vector3(a, b, c);
}

void ParticleSystem::setStartAlpha(float a)
{
	this->startAlpha = a;
}

void ParticleSystem::setEndAlpha(float a)
{
	this->endAlpha = a;
}

void ParticleSystem::setParticlesPerFrame(int i)
{
	this->ParticlesPerFrame = i;
}

void ParticleSystem::setGravity(bool b)
{
	this->isGravityOn = b;
}

void ParticleSystem::setRadius(float r)
{
	this->radius = r;
}

void ParticleSystem::setNumParticles(int num)
{
	this->numParticles = num;
}

Mesh* ParticleSystem::getMesh(void) const
{
	return this->mesh;
}

Vector3 ParticleSystem::getStartPos(void) const
{
	return this->StartPos;
}

Vector3 ParticleSystem::getStartSpeed(void) const
{
	return this->StartSpeed;
}

Vector3 ParticleSystem::getEndSpeed(void) const
{
	return this->EndSpeed;
}

Vector3 ParticleSystem::getStartScale(void) const
{
	return this->StartScale;
}

Vector3 ParticleSystem::getEndScale(void) const
{
	return this->EndScale;
}

float ParticleSystem::getStartAlpha(void) const
{
	return this->startAlpha;
}

float ParticleSystem::getEndAlpha(void) const
{
	return this->endAlpha;
}

int ParticleSystem::getNumPartPerFrame(void) const
{
	return this->ParticlesPerFrame;
}

bool ParticleSystem::getGravity(void) const
{
	return this->isGravityOn;
}

float ParticleSystem::getRadius(void) const
{
	return this->radius;
}

int ParticleSystem::getNumParticles(void) const
{
	return this->numParticles;
}

std::vector<Particle*>& ParticleSystem::getPartlist()
{
	return this->particles;
}

void ParticleSystem::pushPart(Particle* part)
{
	this->particles.push_back(part);
}

void ParticleSystem::popPart(Particle* part)
{
	for (unsigned i = 0; i < particles.size(); ++i) {
		if (particles.at(i) == part) {
			particles.erase(particles.begin() + i);
			break;
		}
	}
}
