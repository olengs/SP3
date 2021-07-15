#pragma once
#include "Particle.h"
#include<vector>
class ParticleSystem
{
public:
	enum Particle_type {
		P_NONE = 0,
		P_ONE,
		P_RAIN,
		P_FOUNTAIN,
		P_FIREWORK,
		P_TOTAL
	};
private:
	int numParticles;
	Mesh* mesh;
	Vector3 StartPos;
	Vector3 StartSpeed;
	Vector3 EndSpeed;
	Vector3 StartScale;
	Vector3 EndScale;

	float startAlpha;
	float endAlpha;
	int ParticlesPerFrame;
	bool isGravityOn;
	float radius;
	std::vector<Particle*> particles;
public:
	bool isactive;
	float ftime;
	float spawntime;
	Particle_type type;
	bool isadditive;
	ParticleSystem();
	~ParticleSystem();
	
	void setMesh(Mesh* mesh);
	void setStartPos(Vector3& pos);
	void setStartPos(float a = 0, float b = 0, float c = 0);
	void setStartSpeed(Vector3& speed);
	void setStartSpeed(float a = 0, float b = 0, float c = 0);
	void setEndSpeed(Vector3& speed);
	void setEndSpeed(float a = 0, float b = 0, float c = 0);
	void setStartScale(Vector3& scale);
	void setStartScale(float a = 0, float b = 0, float c = 0);
	void setEndScale(Vector3& scale);
	void setEndScale(float a = 0, float b = 0, float c = 0);

	void setStartAlpha(float a);
	void setEndAlpha(float a);
	void setParticlesPerFrame(int i);
	void setGravity(bool b);
	void setRadius(float r);
	void setNumParticles(int num);

	Mesh* getMesh(void) const;
	Vector3 getStartPos(void) const;
	Vector3 getStartSpeed(void) const;
	Vector3 getEndSpeed(void) const;
	Vector3 getStartScale(void) const;
	Vector3 getEndScale(void) const;

	float getStartAlpha(void) const;
	float getEndAlpha(void) const;
	int getNumPartPerFrame(void) const;
	bool getGravity(void) const;
	float getRadius(void) const;
	int getNumParticles(void) const;
	std::vector<Particle*>& getPartlist();
	void pushPart(Particle* part);
	void popPart(Particle* part);
};

