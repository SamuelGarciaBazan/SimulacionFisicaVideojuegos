#pragma once
#include <list>

#include "Particle.h"




class Scene
{
public:
	Scene() noexcept {};
	virtual ~Scene() {};

	virtual void update(double t) = 0;

	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera) {};

protected:
	std::list<Particle*> allParticles;
};

