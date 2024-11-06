#pragma once
#include "ForceGenerator.h"


class ExplosionForceGenerator :
    public ForceGenerator
{
public:
    ExplosionForceGenerator(std::list<Particle*>& particlesList, bool check = false);

    virtual ~ExplosionForceGenerator();


protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);
    virtual bool afectCondition(Particle* target);


};

