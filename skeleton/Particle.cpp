#include "Particle.h"

#include <cmath>
#include <iostream>

using namespace physx;


const double Particle::defaultGravityY = -99.8;



Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acel,double damping ,
	PxGeometryType::Enum type )
	:transform(pos), vel(vel), acel(acel),damping(damping)
{

	PxGeometry* g ;
	PxShape* shape;
	
	switch (type)
	{
	case physx::PxGeometryType::eSPHERE:
		g = new PxSphereGeometry(1);
		break;
	case physx::PxGeometryType::eCAPSULE:
		g = new PxCapsuleGeometry(1,2);
		break;
	case physx::PxGeometryType::eBOX:
		g = new PxBoxGeometry(1, 1, 1);
		break;

	default:
		g = new PxSphereGeometry(1);
		break;
	}
	
	shape = CreateShape(*g);
	
	PxVec4 color{ 1,1,1,1 };

	renderItem = new RenderItem(shape, &transform, color);


	delete g;

}




Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	//std::cout << damping << std::endl;
	//std::cout << std::pow(damping, t) << std::endl;
	//std::cout << (acel * t).x << std::endl;

	vel = vel * std::pow(damping, t);
	vel += acel * t + Vector3(0,1,0)*gravityY*t;
	transform.p += vel * t;
	
}

void Particle::scaleObject(double realVel, double realMas, double scaleFactor)
{

	double newVel;

	newVel = realVel * scaleFactor;

	const double realVelSquare = std::pow(realVel, 2);
	const double newVelSquare = std::pow(newVel, 2);
	

	mass = realVelSquare / newVelSquare * realMas;


	gravityY = defaultGravityY * newVelSquare / realVelSquare;
	
	
	vel*= scaleFactor;

}

void Particle::setFromCamera()
{
	this->transform.p = GetCamera()->getTransform().p;
	this->vel = GetCamera()->getDir() * this->vel.magnitude();
}
