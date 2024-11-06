#pragma once
#include "WindForceGenerator.h"

class TornadoForceGenerator :
    public WindForceGenerator
{

public:

    TornadoForceGenerator(std::list<Particle*>& particlesList, bool check = false);

    virtual ~TornadoForceGenerator();


    virtual void setMinRange(const physx::PxVec3& point) {
        minRange = point;
        updateCenter();
    }
    virtual void setMaxRange(const physx::PxVec3& point) {
        maxRange = point;
        updateCenter();
    }

private:

    //se llama cuando se cambia el rango
    void updateCenter();

protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);

    physx::PxVec3 center;

    double k;//constante de fuerza del tornado

};

