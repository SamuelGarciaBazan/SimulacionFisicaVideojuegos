#pragma once
#include "Scene.h"

#include "ExplosionForceGenerator.h"


class ExplosionScene :
    public Scene
{
public:
    ExplosionScene();
    virtual ~ExplosionScene();

    virtual void update(double t);

    virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);

private:

    void createWaterJetSystem();

    ExplosionForceGenerator* explosionGen = nullptr;
    ParticleSystem* particleSystemWaterJet = nullptr;
    Particle* waterJetModel = nullptr;
};

