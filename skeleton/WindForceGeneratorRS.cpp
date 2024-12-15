#include "WindForceGeneratorRS.h"
#include <iostream>

WindForceGeneratorRS::WindForceGeneratorRS(std::list<RigidSolid*>& particlesList, bool check)
	:ForceGeneratorRS(particlesList, check)
{
}

WindForceGeneratorRS::~WindForceGeneratorRS()
{
}

physx::PxVec3 WindForceGeneratorRS::forceCalculation(RigidSolid* target)
{
	physx::PxVec3 relativeVel = velocity - target->getPxRigidDynamic()->getLinearVelocity();

	//physx::PxVec3 force = (k1 * relativeVel) + (k2 * (relativeVel.magnitude() * relativeVel));
	//std::cout << "windForce [ x:" << force.x << " y: " << force.y << " z: " << force.z << " ]" << std::endl;


	//rozamiento normal + rozamiento cuadratico con turbulencias
	return (k1 * relativeVel) + (k2 * (relativeVel.magnitude() * relativeVel));
}

//return true if target is in the area of the wind
bool WindForceGeneratorRS::afectCondition(RigidSolid* target)
{
	auto pos = target->getPxRigidDynamic()->getGlobalPose().p;

	return (pos.x > minRange.x && pos.x < maxRange.x &&
		pos.y > minRange.y && pos.y < maxRange.y &&
		pos.z > minRange.z && pos.z < maxRange.z);
}
