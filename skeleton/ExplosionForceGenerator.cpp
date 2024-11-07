#include "ExplosionForceGenerator.h"

#include <iostream>


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
		std::pow(tPos.x - center.x, 2)+
		std::pow(tPos.y - center.y, 2) + 
		std::pow(tPos.z - center.z, 2)
	);

	//std::cout << "aa" << std::endl;

	
	//se hace aqui para ahorrar el calculo de la distancia
	if (distance < radius) {
		physx::PxVec3 dir(	tPos.x - center.x, 
							tPos.y - center.y,
							tPos.z - center.z);

		dir.normalizeSafe();

		dir = (k / std::pow(distance,2) ) * dir * std::exp(-timeCounter / timeConstant);
		

		return dir;
	}
	else return physx::PxVec3(0,0,0);

}

bool ExplosionForceGenerator::afectCondition(Particle* target)
{
	return true;
}
