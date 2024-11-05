#pragma once
#include "ForceGenerator.h"



class GravityForceGenerator :
    public ForceGenerator
{
public:
    GravityForceGenerator(std::list<ParticleSystem*>& particleSystemList, bool check = false);

    virtual ~GravityForceGenerator();


    double getAceleration() noexcept {
        return aceleration;
    }

    void setAceleration(double v) noexcept {
        aceleration = v;
    }

protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);
    
    double aceleration = -9.8;//default = -9.8

};

