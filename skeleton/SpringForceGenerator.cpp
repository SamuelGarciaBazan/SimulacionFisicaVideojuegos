#include "SpringForceGenerator.h"
#include "SpringForceGenerator.h"




SpringForceGenerator::SpringForceGenerator(Particle* afectedParticle, Particle* source, bool check)
	:ForceGenerator(afectedParticle,check), source(source)
{
}

physx::PxVec3 SpringForceGenerator::forceCalculation(Particle* target)
{
	auto pos = source == nullptr ? position : source->getPos();

	physx::PxVec3 dir = target->getPos() - pos;

	//formula de la ley de Hooke
	return -k * (dir.magnitude() - reposeLenght) * (dir / dir.magnitude());
}

bool SpringForceGenerator::afectCondition(Particle* target)
{
	return true;
}
