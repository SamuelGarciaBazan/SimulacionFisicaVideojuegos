#include "TornadoForceGenerator.h"

TornadoForceGenerator::TornadoForceGenerator(std::list<Particle*>& particlesList, bool check)
	:WindForceGenerator(particlesList, check)
{
}

TornadoForceGenerator::~TornadoForceGenerator()
{
}

void TornadoForceGenerator::updateCenter()
{
	center = {	(minRange.x + maxRange.x)/2,
				(minRange.y + maxRange.y)/2,
				(minRange.z + maxRange.z)/2
											};
}

physx::PxVec3 TornadoForceGenerator::forceCalculation(Particle* target)
{
	auto tPos = target->getPos();

	velocity =
	{


	};

	return physx::PxVec3();
}
