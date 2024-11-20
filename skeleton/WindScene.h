#pragma once
#include "Scene.h"





class WindScene :
    public Scene
{
public:
    WindScene();
    virtual ~WindScene();

    virtual void update(double t);

private:

    ParticleSystem* particleSystemSnow = nullptr;
    Particle* snowModel = nullptr;

};

