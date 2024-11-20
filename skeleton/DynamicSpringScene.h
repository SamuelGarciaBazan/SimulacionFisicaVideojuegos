#pragma once
#include "Scene.h"
#include "SpringForceGenerator.h"


class DynamicSpringScene :
    public Scene
{
public:
    DynamicSpringScene();
    virtual ~DynamicSpringScene();

    virtual void update(double t);

private:
    SpringForceGenerator* dynamicSpringGenA = nullptr;
    SpringForceGenerator* dynamicSpringGenB = nullptr;
    Particle* dynamicSpringParticleA = nullptr;
    Particle* dynamicSpringParticleB = nullptr;

};

