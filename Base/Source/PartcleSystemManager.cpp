#include "PartcleSystemManager.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include <TemplatedFunc.h>


ParticleSystemManager::ParticleSystemManager()
{
	for (int i = 0; i < 800; ++i) {
		Particle* p = new Particle();
		p->active = false;
		InactiveParticles.push_back(p);
	}
}

ParticleSystemManager::~ParticleSystemManager()
{
	while (InactiveParticles.size() > 0) {
		Particle* part = InactiveParticles.back();
		delete part;
		part = nullptr;
		InactiveParticles.pop_back();
	}
	for (std::vector<ParticleSystem*>::iterator it = Partsystems.begin(); it != Partsystems.end(); ++it) {
		ParticleSystem* sys = (ParticleSystem*)*it;
		delete sys->getMesh();
	}
	while (Partsystems.size() > 0) {
		ParticleSystem* sys = Partsystems.back();
		delete sys;
		sys = nullptr;
		Partsystems.pop_back();
	}
}

void ParticleSystemManager::setCamera(Camera3*& camera)
{
	this->camera = camera;
}

void ParticleSystemManager::Update(double dt)
{
	for (std::vector<ParticleSystem*>::iterator it = Partsystems.begin(); it != Partsystems.end(); ++it) {
		ParticleSystem* sys = (ParticleSystem*)*it;
		if (sys->ftime > sys->spawntime && sys->isactive) {
			sys->ftime -= sys->spawntime;
			for (int i = 0; i < sys->getNumPartPerFrame(); ++i) {
				Particle* p = FetchPart();
				if (p->active)
				{
					continue;
				}
				p->active = true;
				p->mesh = sys->getMesh();
				p->transform.translate = sys->getStartPos() + Vector3(Math::RandFloatMinMax(-sys->getRadius(), sys->getRadius()), 0, Math::RandFloatMinMax(-sys->getRadius(), sys->getRadius()));
				p->transform.Scale = sys->getStartScale();
				p->vel = sys->getStartSpeed();
				p->alpha = sys->getStartAlpha();
				p->hsv.Hue = 0;
				p->ftime = 0;
				if (!Algorithm::SearchnSort::in(p, sys->getPartlist())) {
					sys->pushPart(p);
				}
			}
		}
 		for (std::vector<Particle*>::iterator it = sys->getPartlist().begin(); it != sys->getPartlist().end();++it ) {
			Particle* p = (Particle*)*it;
			if (!p->active) {
				continue;
			}
			if (p->mesh != sys->getMesh()) {
				continue;
			}
			switch (sys->type) {
			case ParticleSystem::Particle_type::P_RAIN:
			{
				p->transform.translate += p->vel * (float)dt;
				p->vel += Vector3(0,-9.8f,0) * (float)dt;
				p->transform.Scale -= Vector3(1.5, 1.5, 1.5) * (float)dt;
				p->alpha -= 0.15f * (float)dt;
				if (p->alpha < 0.f) {
					p->active = false;
				}
				break;
			}
			case ParticleSystem::Particle_type::P_FOUNTAIN:
			{
				p->transform.translate += p->vel * (float)dt;
				if (p->ftime < 2.f || p->ftime > 4.f) {
					p->vel += Vector3(0, -2.8f, 0) * (float)dt;
					p->transform.Scale -= Vector3(0.7f, 0.7f, 0.7f) * (float)dt;
					p->transform.translate += p->vel * (float)dt;
					p->ftime += (float)dt;
				}
				else {
					float angle = Math::RandFloatMinMax(0, Math::TWO_PI);
					float xvel = Math::RandFloatMinMax(0, 1.f);
					p->vel += Vector3(xvel * cosf(angle),0,xvel * sinf(angle));
					p->ftime += 3.f;
				}
				if (abs(p->transform.translate.x) > terrain.Scale.x / 2 || abs(p->transform.translate.z) > terrain.Scale.z / 2
					|| p->transform.translate.y < ReadHeightMap(m_heightMap, p->transform, terrain)) {
					p->active = false;
					p->vel = Vector3(0, 0, 0);
				}
				break;

			}
			case ParticleSystem::Particle_type::P_FIREWORK:
			{
				p->transform.translate += p->vel * (float)dt;
				if (p->ftime > 4.f) {
					p->hsv.Hue += (float)dt * 120.f;
					p->hsv.Hue = fmod(p->hsv.Hue, 360.f);
				}
				if (p->ftime < 2.f || p->ftime > 4.f) {
					p->vel += Vector3(0, -4.1f, 0) * (float)dt;
					p->transform.Scale -= Vector3(0.9f, 0.9f, 0.9f) * (float)dt;
					p->transform.translate += p->vel * (float)dt;
					p->ftime += (float)dt;
				}
				else {
					float angle = Math::RandFloatMinMax(0, Math::TWO_PI);
					float xvel = 5.f;
					p->vel += Vector3(xvel * cosf(angle), xvel * sinf(angle),0) ;
					p->ftime += 3.f;
				}
				if (p->transform.Scale.x <= 0.2f) {
					p->active = false;
					p->vel = Vector3(0, 0, 0);
				}
			}
			}
			p->transform.rotateAngles.y =
				Math::RadianToDegree(atan2(camera->position.x - p->transform.translate.x, camera->position.z - p->transform.translate.z));
			p->DistFromCamSquare = (p->transform.translate - camera->position).LengthSquared();
		}
		sys->ftime += (float)dt;
	}
}

void ParticleSystemManager::setTerrain(TRS terrain)
{
	this->terrain = terrain;
}

void ParticleSystemManager::setHeightMap(std::vector<unsigned char> heightmap)
{
	this->m_heightMap = heightmap;
}

Particle* ParticleSystemManager::FetchPart()
{
	for (std::vector<Particle*>::iterator it = InactiveParticles.begin(); it != InactiveParticles.end(); ++it) 
	{
		Particle* p = (Particle*)*it;
		if (!p->active) {
			return p;
		}
	}
	for (int i = 0; i < 10; ++i) {
		Particle* part = new Particle();
		part->active = false;
		InactiveParticles.push_back(part);
	}
	return FetchPart();
	//return InactiveParticles.back();
}

std::vector<ParticleSystem*>& ParticleSystemManager::getSysList()
{
	return this->Partsystems;
}

void ParticleSystemManager::pushSys(ParticleSystem* sys)
{
	this->Partsystems.push_back(sys);
}

std::vector<Particle*>& ParticleSystemManager::getAllPart()
{
	// TODO: insert return statement here
	return this->InactiveParticles;
}
