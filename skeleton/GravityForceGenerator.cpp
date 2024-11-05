#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(std::list<ParticleSystem*>& particleSystemList, bool check)
	:ForceGenerator(particleSystemList, check)
{
}

GravityForceGenerator::~GravityForceGenerator()
{
}

physx::PxVec3 GravityForceGenerator::forceCalculation(Particle* target)
{
	return physx::PxVec3(0, target->getMass() * aceleration,0); 
}
