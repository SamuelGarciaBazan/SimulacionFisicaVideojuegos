#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(std::list<ParticleSystem*>& particleSystemList, bool check)
	:ForceGenerator(particleSystemList, check)
{
}

GravityForceGenerator::~GravityForceGenerator()
{
}

double GravityForceGenerator::forceCalculation(Particle* target)
{
	return target->getMass() * aceleration;
}
