#pragma once

#include <PxPhysicsAPI.h>

#include "RenderUtils.hpp"

#include <list>


class Particle
{
public:
	Particle(
		std::list<Particle*>& allParticles,
		physx::PxVec3 pos,
		physx::PxQuat quat,
		physx::PxVec3 vel, 
		double scale = 1.0,
		double damping = 1.0, 
		double mass = 1,
		physx::PxGeometryType::Enum type = physx::PxGeometryType::Enum::eSPHERE, 
		physx::PxVec4 color = physx::PxVec4(1,1,1,1));
	~Particle();

	void integrate(double t);

	void scaleObject(double realVel, double realMas, double scaleFactor);

	void setFromCamera();

	static const double defaultGravityY;

	physx::PxVec3 getPos() const { return transform.p; }

	double getDamping() const noexcept { return damping; }
	double getMass() const noexcept { return mass; }

	physx::PxGeometryType::Enum getGeometryType() const { return geometryType; }
	physx::PxVec4 getColor() const { return color; }

	physx::PxQuat getQuat() const { return transform.q; }

private:

	physx::PxVec3 vel;
	physx::PxVec3 acel;
	physx::PxTransform transform;

	double damping;
	
	double mass;
	
	double gravityY = defaultGravityY;

	physx::PxGeometryType::Enum geometryType;

	physx::PxVec4 color;

	double scale;

	RenderItem* renderItem;

	std::list<Particle*>& allParticles;
};

