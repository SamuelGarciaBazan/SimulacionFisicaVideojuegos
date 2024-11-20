#pragma once
#include <list>

#include "Particle.h"



class Scene
{
public:
	Scene() noexcept {};
	virtual ~Scene() {};

	virtual void update(double t) = 0;

protected:
	std::list<Particle*> allParticles;
};

