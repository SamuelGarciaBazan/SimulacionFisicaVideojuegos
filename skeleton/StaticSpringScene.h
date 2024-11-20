#pragma once
#include "Scene.h"
#include "SpringForceGenerator.h"

class StaticSpringScene :
    public Scene
{
public:

	StaticSpringScene();
	virtual ~StaticSpringScene() ;

	virtual void update(double t);

private:

	SpringForceGenerator* staticSpringGen = nullptr;
	Particle* staticSpringParticle = nullptr;
};

