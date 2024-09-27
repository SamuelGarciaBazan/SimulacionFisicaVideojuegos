#pragma once

#include <PxPhysicsAPI.h>

#include "RenderUtils.hpp"
#include "Vector3D.h"



class Particle
{
public:
	Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acel);
	~Particle();

	void integrate(double t);



private:

	physx::PxVec3 vel;
	physx::PxVec3 acel;
	physx::PxTransform transform;

	RenderItem* renderItem;


};

