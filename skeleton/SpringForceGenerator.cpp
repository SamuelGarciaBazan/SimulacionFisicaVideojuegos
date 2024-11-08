#include "SpringForceGenerator.h"
#include "SpringForceGenerator.h"




physx::PxVec3 SpringForceGenerator::forceCalculation(Particle* target)
{
	auto pos = source == nullptr ? position : source->getPos();

	physx::PxVec3 dir = target->getPos() - pos;

	return -k * (dir.magnitude() - reposeLenght) * (dir/ dir.magnitude());
}

bool SpringForceGenerator::afectCondition(Particle* target)
{
	return true;
}
