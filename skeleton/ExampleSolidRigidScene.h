#pragma once
#include "Scene.h"
#include "tiny_obj_loader.h"


class ExampleSolidRigidScene :
    public Scene
{

public:
    ExampleSolidRigidScene(physx::PxPhysics* gPhysics,physx::PxScene* gScene,physx::PxCooking* gCooking);
    virtual ~ExampleSolidRigidScene();

    virtual void update(double t);


    virtual void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

private:

    //instancia de physics y de escena, necesarias para solidos rigidos
    //gPhysics para la creacion del solido rigido
    //gScene para aniadirlo a la escena
    physx::PxPhysics* gPhysics;
    physx::PxScene* gScene;
    physx::PxCooking* gCooking;

    RenderItem* floorRenderItem;
    RenderItem* cubeRenderItem;
    RenderItem* balaRenderItem;



    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

};

