#pragma once

#include <vector>
#include "Particle.h"

class ParticleSystem
{
public:

	ParticleSystem();
	~ParticleSystem();

	enum RandomMode {
		UNIFORM,NORMAL
	};

private:

	Particle* model;

	std::vector<Particle*> particles;

	double startLifeTimeMinRange;
	double startLifeTimeMaxRange;

	physx::PxVec3 lifePosMinRange;
	physx::PxVec3 lifePosMaxRange;

	physx::PxVec3 velMinRange;
	physx::PxVec3 velMaxRange;

	physx::PxVec3 posMinRange;
	physx::PxVec3 posMaxRange;

	RandomMode randomMode;

};

