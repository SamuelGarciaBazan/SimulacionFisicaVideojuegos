#pragma once
#include "ForceGenerator.h"



class BouyancyForceGenerator :
    public ForceGenerator
{
public:

	BouyancyForceGenerator(Particle* afectedParticle, bool check = false);


	void setLiquidParticle(Particle* p) {
		_liquid_particle = p;
	}

protected:

	virtual physx::PxVec3 forceCalculation(Particle* target);
	virtual bool afectCondition(Particle* target);


	float _height = 2;
	float _volume = 1;
	float _liquid_density = 1000;
	float _gravity = 9.8;

	Particle* _liquid_particle;

};

