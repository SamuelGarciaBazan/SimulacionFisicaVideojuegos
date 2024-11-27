#pragma once
#include "Scene.h"



class ExampleSolidRigidScene :
    public Scene
{

public:
    ExampleSolidRigidScene(physx::PxPhysics* gPhysics,physx::PxScene* gScene);
    virtual ~ExampleSolidRigidScene();

    virtual void update(double t);


    virtual void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

private:

    //instancia de physics y de escena, necesarias para solidos rigidos
    //gPhysics para la creacion del solido rigido
    //gScene para aniadirlo a la escena
    physx::PxPhysics* gPhysics;
    physx::PxScene* gScene;


    RenderItem* floorRenderItem;
    RenderItem* cubeRenderItem;

};

