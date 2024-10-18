#pragma once

#include <list>
#include "Particle.h"

class ParticleSystem
{
public:

	ParticleSystem();
	~ParticleSystem();

	enum RandomMode {
		UNIFORM,NORMAL
	};


	void update(double d);

private:

	struct ParticleData
	{
		//la posicion la sacamos de la particula
		Particle* particle;
		//tiempo de vida de la particula
		double currentLifeTime;
	};

	Particle* model;

	std::list<ParticleData> particles;

	int maxParticles;

	double creationRate;
	double currentCreationTimer;


	double startLifeTimeMinRange;
	double startLifeTimeMaxRange;

	physx::PxVec3 lifePosMinRange;
	physx::PxVec3 lifePosMaxRange;

	physx::PxVec3 velMinRange;
	physx::PxVec3 velMaxRange;

	physx::PxVec3 startPosMinRange;
	physx::PxVec3 startPosMaxRange;

	RandomMode randomMode;

	bool loop;
	double duration;
	double delay;






	void deleteParticles(double d);

	bool mustDie(ParticleData p);


	void createNewParticles(double d);

	void createParticle();

};

