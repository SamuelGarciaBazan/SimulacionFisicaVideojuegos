#include "BouyancyForceGenerator.h"

BouyancyForceGenerator::BouyancyForceGenerator(Particle* afectedParticle, bool check)
	:ForceGenerator(afectedParticle,check)
{


}



physx::PxVec3 BouyancyForceGenerator::forceCalculation(Particle* target)
{

	float h = target->getPos().y;
	float h0 = _liquid_particle->getPos().y;

	Vector3 f(0, 0, 0);
	float immersed = 0.0;

	if (h - h0 > _height * 0.5) {
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}

	f.y = _liquid_density * _volume * immersed * _gravity;

	return f;
}

bool BouyancyForceGenerator::afectCondition(Particle* target)
{
	return true;
}
