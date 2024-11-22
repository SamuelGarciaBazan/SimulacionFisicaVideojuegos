#pragma once
#include "SpringForceGenerator.h"


class ElasticBandForceGenerator :
    public SpringForceGenerator
{

public:

    ElasticBandForceGenerator(Particle* afectedParticle, Particle* source = nullptr, bool check = false);

protected:
    virtual physx::PxVec3 forceCalculation(Particle* target);


};

