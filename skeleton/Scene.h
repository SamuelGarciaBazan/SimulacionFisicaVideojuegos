#pragma once
#include <list>

#include "Particle.h"
#include "ParticleSystem.h"

#include "RigidSolid.h"


class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};


	
	virtual void update(double t) = 0;


	//manejo de input
	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera) {};

	virtual void keyboardUp(unsigned char key, const physx::PxTransform& camera) {};
	virtual void specialKeyDown(int key, const physx::PxTransform& camera) {};
	virtual void specialKeyUp(int key, const physx::PxTransform& camera) {};

	virtual void joystickInput(unsigned int buttonMask, int x, int y, int z, const physx::PxTransform& camera) {};

	//colisiones
	virtual void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {};

protected:
	std::list<Particle*> allParticles;
	std::vector<ParticleSystem*> particlesSystems;


	std::list<RigidSolid*> allRigidSolids;
};

