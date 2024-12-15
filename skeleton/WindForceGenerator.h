#pragma once
#include "ForceGenerator.h"


class WindForceGenerator :
    public ForceGenerator
{
public:
    WindForceGenerator(std::list<Particle*>& particlesList, bool check = false);

    WindForceGenerator(ParticleSystem* particleSystem, bool check = false);

    virtual ~WindForceGenerator();

#pragma region Getters y setters


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

    const physx::PxVec3& getMinRange() const noexcept {
        return minRange;
    }
    virtual void setMinRange(const physx::PxVec3& point) {
        minRange = point;
    }
    const physx::PxVec3& getMaxRange() const noexcept {
        return maxRange;
    }
    virtual void setMaxRange(const physx::PxVec3& point) {
        maxRange = point;
    }

#pragma endregion


protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);
    virtual bool afectCondition(Particle* target);


    physx::PxVec3 velocity { 0,0,0 };

    physx::PxVec3 minRange { 0,0,0 };
    physx::PxVec3 maxRange { 0,0,0 };

    double k1 = 0.7;//coeficiente de rozamiento del aire + forma del objeto
    double k2 = 0;//coeficiente para ajustar la turbulencia a altas velocidades
};

