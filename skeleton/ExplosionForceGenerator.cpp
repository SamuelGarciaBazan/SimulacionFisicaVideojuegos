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
	auto tPos = target->getPos();

	double distance = std::sqrt( 
		std::pow(tPos.x -center.x,2)+
		std::pow(tPos.y - center.y, 2) + 
		std::pow(tPos.z - center.z, 2));

	if (distance < radius) {

		std::exp(-timeCounter / timeConstant);



		return physx::PxVec3(0, 0, 0);
	}
	else return physx::PxVec3(0,0,0);

}

bool ExplosionForceGenerator::afectCondition(Particle* target)
{
	return true;
}
