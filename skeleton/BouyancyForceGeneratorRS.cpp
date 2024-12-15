#include "BouyancyForceGeneratorRS.h"


BouyancyForceGeneratorRS::BouyancyForceGeneratorRS(RigidSolid* afectedParticle, bool check)
	:ForceGeneratorRS(afectedParticle, check)
{


}

BouyancyForceGeneratorRS::BouyancyForceGeneratorRS(std::list<RigidSolid*>& afectedRigidSolids, bool check)
	:ForceGeneratorRS(afectedRigidSolids,check)
{
}



physx::PxVec3 BouyancyForceGeneratorRS::forceCalculation(RigidSolid* target)
{

	float h = target->getPxRigidDynamic()->getGlobalPose().p.y;
	
	RigidSolid::MyData* data =  (RigidSolid::MyData*)(target->getPxRigidDynamic()->userData);

	_height = data->height;
	_volume = data->volume;

	//float h0 = _liquid_particle->getPos().y;

	float h0 = 0;

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

bool BouyancyForceGeneratorRS::afectCondition(RigidSolid* target)
{
	return true;
}
