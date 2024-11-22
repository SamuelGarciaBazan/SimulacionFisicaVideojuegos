#pragma once
#include "ForceGenerator.h"



class BouyancyForceGenerator :
    public ForceGenerator
{
public:

	BouyancyForceGenerator(Particle* afectedParticle, bool check = false);


protected:

	virtual physx::PxVec3 forceCalculation(Particle* target);


	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle;

};

