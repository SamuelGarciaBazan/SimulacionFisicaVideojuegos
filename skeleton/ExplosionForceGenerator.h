#pragma once
#include "ForceGenerator.h"


class ExplosionForceGenerator :
    public ForceGenerator
{
public:
    ExplosionForceGenerator(std::list<Particle*>& particlesList, bool check = false);


    virtual ~ExplosionForceGenerator();

    void updateTime(double t) {
        timeCounter += t;
    }

protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);
    virtual bool afectCondition(Particle* target);


    double timeCounter = 0;

    double radius = 0;          //radio al que afecta
    physx::PxVec3 center{0,0,0};   //centro de la explosion
    double k;               //intensidad de la explosion

    double timeConstant = 0.5; //cte de tiempo de la explosion, cuando pasan 4t, la explosion ha terminado(aprox)
};

