#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(std::list<Particle*>& particlesList, bool check)
	:ForceGenerator(particlesList,check)
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

