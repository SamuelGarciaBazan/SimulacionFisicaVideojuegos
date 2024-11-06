#include "ExplosionForceGenerator.h"


ExplosionForceGenerator::ExplosionForceGenerator(std::list<Particle*>& particlesList, bool check)
	:ForceGenerator(particlesList,check)
{
}

ExplosionForceGenerator::~ExplosionForceGenerator()
{
}

physx::PxVec3 ExplosionForceGenerator::forceCalculation(Particle* target)
{




	return physx::PxVec3();
}

bool ExplosionForceGenerator::afectCondition(Particle* target)
{
	return true;
}
