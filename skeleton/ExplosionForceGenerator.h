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

#pragma region Getters y setters

    void setRadius(double r) {
        radius = r;
    }
    double getRadius() {
        return radius;
    }

    void setCenter(physx::PxVec3 p) {
        center = p;
    }

    physx::PxVec3 getCenter() {
        return center;
    }

    void setK(double d) {
        k = d;
    }

    double getK() {
        return k;
    }

    void setTimeConstant(double d) {
        timeConstant = d;
    }

    double getTimeConstant() {
        return timeConstant;
    }

    double getTimeCounter() {
        return timeCounter;
    }

#pragma endregion


   


protected:

    virtual physx::PxVec3 forceCalculation(Particle* target);
    virtual bool afectCondition(Particle* target);


    double timeCounter = 0;

    double radius = 0;          //radio al que afecta
    physx::PxVec3 center {0,0,0};   //centro de la explosion
    double k = 0;               //intensidad de la explosion

    double timeConstant = 0.5; //cte de tiempo de la explosion, cuando pasan 4t, la explosion ha terminado(aprox)
};

