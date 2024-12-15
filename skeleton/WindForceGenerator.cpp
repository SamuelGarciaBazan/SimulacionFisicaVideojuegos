#include "WindForceGenerator.h"

#include <iostream>

WindForceGenerator::WindForceGenerator(std::list<Particle*>& particlesList, bool check)
	:ForceGenerator(particlesList,check)
{
}

WindForceGenerator::WindForceGenerator(ParticleSystem* particleSystem, bool check)
	:ForceGenerator(particleSystem, check)
{
}

WindForceGenerator::~WindForceGenerator()
{
}

physx::PxVec3 WindForceGenerator::forceCalculation(Particle* target)
{
	physx::PxVec3 relativeVel = velocity - target->getVelocity();

	//rozamiento normal + rozamiento cuadratico con turbulencias
	return (k1 * relativeVel) + (k2 *(relativeVel.magnitude() * relativeVel));
}

//return true if target is in the area of the wind
bool WindForceGenerator::afectCondition(Particle* target)
{
	auto pos = target->getPos();

	return (pos.x > minRange.x && pos.x < maxRange.x &&
			pos.y > minRange.y && pos.y < maxRange.y &&
			pos.z > minRange.z && pos.z < maxRange.z);
}

