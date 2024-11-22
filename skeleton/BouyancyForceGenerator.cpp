#include "BouyancyForceGenerator.h"

BouyancyForceGenerator::BouyancyForceGenerator(Particle* afectedParticle, bool check)
	:ForceGenerator(afectedParticle,check)
{

	_liquid_particle  = new Particle(
		allParticles,
		{ 0,15,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		1, //scale
		1, //damping
		1,//mass
		physx::PxGeometryType::eSPHERE,
		{ 1,0,0,1 } // color
	);


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
