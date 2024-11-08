#pragma once
#include "ForceGenerator.h"

#include "Particle.h"

class SpringForceGenerator :
    public ForceGenerator
{
public:

    SpringForceGenerator(Particle* afectedParticle, bool check = false);



#pragma region Getters y setters

    double getK() {
        return k;
    }

    void setK(double d) {
        k = d;
    }

    double getReposeLenght() {
        return reposeLenght;
    }

    void setReposeLenght(double d) {
        reposeLenght = d;
    }

    physx::PxVec3 getPos() {
        return position;
    }

    void setPos(physx::PxVec3 p) {
        position = position;
    }

    void setSourceParticle(Particle* p) {
        source = p;
    }

#pragma endregion



private:

    virtual physx::PxVec3 forceCalculation(Particle* target);
    virtual bool afectCondition(Particle* target);



    double k = 0;           //coeficiente elastico del muelle

    double reposeLenght;//longitud de reposo del muelle

    physx::PxVec3 position{0,0,0};

    Particle* source = nullptr;

};

