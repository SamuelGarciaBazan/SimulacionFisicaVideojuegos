#pragma once

#include "Scene.h"
#include "SpringForceGenerator.h"


class DynamicSpringChainScene :
    public Scene
{
public:
    DynamicSpringChainScene();
    virtual ~DynamicSpringChainScene();

    virtual void update(double t);

private:



    std::vector<SpringForceGenerator> dynamicSpringGenChains;
    std::vector<Particle*> dynamicSpringParticlesChain;
};

