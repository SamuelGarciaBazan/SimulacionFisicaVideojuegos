#include "Particle.h"

#include <cmath>
#include <iostream>

using namespace physx;


const double Particle::defaultGravityY =-9.8;



Particle::Particle(
	std::list<Particle*>& allParticles,
	physx::PxVec3 pos,
	physx::PxQuat quat,
	physx::PxVec3 vel, 
	double scale,
	double damping ,
	double mass,
	PxGeometryType::Enum type,
	physx::PxVec4 color)

	:
	allParticles(allParticles),
	transform(pos),
	vel(vel),acel(PxVec3(0,0,0)),  //inicializacion de parametros
	scale(scale),
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
		geo = new PxSphereGeometry(scale);
		break;
	case physx::PxGeometryType::eCAPSULE:
		geo = new PxCapsuleGeometry(scale,2* scale);
		break;
	case physx::PxGeometryType::eBOX:
		geo = new PxBoxGeometry(scale, scale, scale);
		break;

	default:
		geo = new PxSphereGeometry(1);
		break;
	}
	
	//creacion del shape
	shape = CreateShape(*geo);
	
	//transform.q = PxQuat(1, 0, 1, 1);

	//transform.q.rotate(PxVec3(10,90,90));
	//creacion del renderItem
	renderItem = new RenderItem(shape, &transform, color);

	transform.q = quat;

	delete geo;


	myIt = allParticles.insert(allParticles.end(),this);
}




Particle::~Particle()
{
	allParticles.erase(myIt);

	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	//std::cout << damping << std::endl;
	//std::cout << std::pow(damping, t) << std::endl;
	//std::cout << (acel * t).x << std::endl;

	//std::cout << "Initial vel: " << vel.y << std::endl;


	//std::cout << "Final vel: " << vel.y << std::endl;
	
	/*vel = vel * std::pow(damping, t);
	vel += (acel * t) + (Vector3(0, 1, 0) * gravityY * t);
	transform.p += vel * t;*/
	acel = force / mass;
	vel += acel * t;
	transform.p += vel * t;

	//std::cout << "desplazamiento :"<<vel.y * t << std::endl;


	force = Vector3(0, 0, 0);
	
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


