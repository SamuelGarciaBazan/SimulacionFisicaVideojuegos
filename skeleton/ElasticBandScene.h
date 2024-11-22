#pragma once
#include "Scene.h"

#include "ElasticBandForceGenerator.h"

class ElasticBandScene :
    public Scene
{
public:
    ElasticBandScene();
    virtual ~ElasticBandScene();

    virtual void update(double t);

    virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);

private:
    ElasticBandForceGenerator* dynamicSpringGenA = nullptr;
    ElasticBandForceGenerator* dynamicSpringGenB = nullptr;
    Particle* dynamicSpringParticleA = nullptr;
    Particle* dynamicSpringParticleB = nullptr;

};

