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

    void setKconstant(double d) {
        k = d;
    }

    double getKconstant() {
        return k;
    }

    physx::PxVec3 getForceOffset() {
        return forceOffset;
    }

    void setForceOffset(physx::PxVec3 offset) {
        forceOffset = offset;
    }

private:

    //se llama cuando se cambia el rango
    void updateCenter();

protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);

    physx::PxVec3 center;

    physx::PxVec3 forceOffset{0,50,0};

    double k = 40;//constante de fuerza del tornado

};

