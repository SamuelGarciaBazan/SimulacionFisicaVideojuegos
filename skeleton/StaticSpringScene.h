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

	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera) ;

private:

	SpringForceGenerator* staticSpringGen = nullptr;
	Particle* staticSpringParticle = nullptr;
};

