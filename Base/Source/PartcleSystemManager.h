#pragma once
#include <iostream>
#include "ParticleSystem.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
class ParticleSystemManager
{
public:
	//enum Part_Type {
	//	P_NONE = 0,
	//	P_RAIN,
	//	P_FOUNTAIN,
	//};
private:
	//ParticleSystem* Part_sys;
	//Part_Type type;
	Camera3* camera;
	TRS terrain;
	std::vector<unsigned char> m_heightMap;
	std::vector<Particle*> InactiveParticles;
	std::vector<ParticleSystem*> Partsystems;
public:
	//bool active;
	ParticleSystemManager();
	~ParticleSystemManager();
	//ParticleSystemManager(Part_Type type);
	void setCamera(Camera3*& camera);
	void Update(double dt);
	void setTerrain(TRS terrain);
	void setHeightMap(std::vector<unsigned char> heightmap);
	Particle* FetchPart();
	std::vector<ParticleSystem*>& getSysList();
	void pushSys(ParticleSystem* sys);
	std::vector<Particle*>& getAllPart();
};