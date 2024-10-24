#include "Particle.h"

#include <cmath>
#include <iostream>

using namespace physx;


const double Particle::defaultGravityY = -9.8;



Particle::Particle(
	physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acel,
	double damping ,
	double mass,
	PxGeometryType::Enum type,
	physx::PxVec4 color)

	:transform(pos), vel(vel), acel(acel), //inicializacion de parametros
	damping(damping),
	mass(mass),
	geometryType(type),
	color(color)
{
	//geometry y shape
	PxGeometry* geo;
	PxShape* shape;
	
	//creacion de la geometria dependiendo del tipo
	switch (type)
	{
	case physx::PxGeometryType::eSPHERE:
		geo = new PxSphereGeometry(1);
		break;
	case physx::PxGeometryType::eCAPSULE:
		geo = new PxCapsuleGeometry(1,2);
		break;
	case physx::PxGeometryType::eBOX:
		geo = new PxBoxGeometry(1, 1, 1);
		break;

	default:
		geo = new PxSphereGeometry(1);
		break;
	}
	
	//creacion del shape
	shape = CreateShape(*geo);

	//creacion del renderItem
	renderItem = new RenderItem(shape, &transform, color);

	delete geo;

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

	/*std::cout << "Initial vel: " << vel.y << std::endl;
	vel = vel * std::pow(damping, t);
	vel += (acel * t) + (Vector3(0,1,0) * gravityY * t);

	std::cout << "Final vel: " << vel.y << std::endl;*/


	transform.p += vel * t;
	
}

void Particle::scaleObject(double realVel, double realMas, double scaleFactor)
{

	//modulo de la velocidad
	double newVel;


	newVel = realVel * scaleFactor;

	const double realVelSquare = std::pow(realVel, 2);
	const double newVelSquare = std::pow(newVel, 2);
	

	mass = (realVelSquare / newVelSquare) * realMas;


	gravityY = defaultGravityY * (newVelSquare / realVelSquare);
	
	
	vel*= scaleFactor;
}

void Particle::setFromCamera()
{
	this->transform.p = GetCamera()->getTransform().p;
	this->vel = GetCamera()->getDir() * this->vel.magnitude();
}
