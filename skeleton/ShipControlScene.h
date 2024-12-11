#pragma once
#include "Scene.h"

#include "RigidSolid.h"

class ShipControlScene :
    public Scene
{
public:

    ShipControlScene(physx::PxPhysics* gPhysics, physx::PxScene* gScene);
    virtual ~ShipControlScene();

    virtual void update(double t);

    virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);


private:

    physx::PxPhysics* gPhysics;
    physx::PxScene* gScene;

    RigidSolid* ship;

    physx::PxRigidStatic* floor;
    RenderItem* floorRenderItem;

};

