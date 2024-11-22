#pragma once
#include "Scene.h"

#include "BouyancyForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"

class BouyancyScene :
    public Scene
{
public:
    BouyancyScene();
    virtual ~BouyancyScene();

    virtual void update(double t);

    virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);

private:

    Particle* afectedParticle;
    Particle* liquidParticle;

    WindForceGenerator* windGen;
    BouyancyForceGenerator* bouyancyGenerator;
    GravityForceGenerator* gravityGen;
};

