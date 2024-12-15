#pragma once
#include "ForceGeneratorRS.h"


class BouyancyForceGeneratorRS :
    public ForceGeneratorRS
{
public:

	BouyancyForceGeneratorRS(RigidSolid* afectedParticle, bool check = false);

	BouyancyForceGeneratorRS(std::list<RigidSolid*>& afectedRigidSolids, bool check = false);

	//void setLiquidParticle(RigidSolid* p) {
	//	_liquid_particle = p;
	//}

protected:

	virtual physx::PxVec3 forceCalculation(RigidSolid* target);
	virtual bool afectCondition(RigidSolid* target);


	float _height = 3;
	float _volume = 72;
	float _liquid_density = 1.5;
	float _gravity = 9.8;

	//RigidSolid* _liquid_particle;

};

