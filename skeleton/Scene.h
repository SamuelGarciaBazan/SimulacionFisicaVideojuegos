#pragma once
#include <list>

#include "Particle.h"
#include "ParticleSystem.h"



class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	virtual void update(double t) = 0;

	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera) {};

	virtual void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {};

protected:
	std::list<Particle*> allParticles;
	std::vector<ParticleSystem*> particlesSystems;

};

