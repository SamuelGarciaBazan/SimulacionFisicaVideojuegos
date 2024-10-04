#pragma once

#include <PxPhysicsAPI.h>

#include "RenderUtils.hpp"
#include "Vector3D.h"



class Particle
{
public:
	Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acel,double damping = 1.0,
		physx::PxGeometryType::Enum type = physx::PxGeometryType::Enum::eSPHERE);
	~Particle();

	void integrate(double t);


	void scaleObject(double realVel, double realMas, double scaleFactor);


	void setFromCamera();

	static const double defaultGravityY;

private:

	physx::PxVec3 vel;
	physx::PxVec3 acel;
	physx::PxTransform transform;

	RenderItem* renderItem;

	double damping;



	double mass;
	double gravityY =defaultGravityY;


};

