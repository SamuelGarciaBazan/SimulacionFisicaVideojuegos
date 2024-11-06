#pragma once
#include "ForceGenerator.h"


class WindForceGenerator :
    public ForceGenerator
{
public:
    WindForceGenerator(std::list<Particle*>& particlesList, bool check = false);

    virtual ~WindForceGenerator();



    const physx::PxVec3& getVelocity() const noexcept {
        return velocity;
    }
    void setVelocity(const physx::PxVec3& newVelocity) {
        velocity = newVelocity;
    }

    double getK1() const noexcept {
        return k1;
    }
    void setK1(double newK1) noexcept {
        k1 = newK1;
    }

    double getK2() const noexcept{
        return k2;
    }
    void setK2(double newK2) noexcept {
        k2 = newK2;
    }

protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);

    physx::PxVec3 velocity;

    double k1 = 0.7;//coeficiente de rozamiento del aire + forma del objeto
    double k2 = 0;//coeficiente para ajustar la turbulencia a altas velocidades
};

