#pragma once
#include "Scene.h"

#include "WindForceGenerator.h"



class WindScene :
    public Scene
{
public:
    WindScene();
    virtual ~WindScene();

    virtual void update(double t);

private:

    void createSnowSystem();

    WindForceGenerator* windGen = nullptr;


    ParticleSystem* particleSystemSnow = nullptr;
    Particle* snowModel = nullptr;

};

