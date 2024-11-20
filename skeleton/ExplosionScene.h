#pragma once
#include "Scene.h"

#include "ExplosionForceGenerator.h"
#include "TornadoForceGenerator.h"


class ExplosionScene :
    public Scene
{
public:
    ExplosionScene();
    virtual ~ExplosionScene();

    virtual void update(double t);

    virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);

private:

    void createSnowSystem();
    

    ExplosionForceGenerator* explosionGen = nullptr;
    TornadoForceGenerator* tornadoGen = nullptr;
    ParticleSystem* particleSystemSnow = nullptr;
    Particle* snowModel = nullptr;
};

