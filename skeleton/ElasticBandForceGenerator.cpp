#include "ElasticBandForceGenerator.h"

ElasticBandForceGenerator::ElasticBandForceGenerator(Particle* afectedParticle, Particle* source, bool check)
	:SpringForceGenerator(afectedParticle, source, check)

{
}

physx::PxVec3 ElasticBandForceGenerator::forceCalculation(Particle* target)
{
	auto pos = source == nullptr ? position : source->getPos();

	physx::PxVec3 dir = target->getPos() - pos;

	if (dir.magnitude() <= reposeLenght) {
		return { 0,0,0 };
	}
	//formula de la ley de Hooke
	return -k * (dir.magnitude() - reposeLenght) * (dir / dir.magnitude());
}
