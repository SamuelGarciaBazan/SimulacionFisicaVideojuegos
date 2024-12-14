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

    virtual void specialKeyDown(int key, const physx::PxTransform& camera);
    virtual void specialKeyUp(int key, const physx::PxTransform& camera);

private:

    void updateMove(double t);



    physx::PxPhysics* gPhysics;
    physx::PxScene* gScene;

    RigidSolid* ship;

    physx::PxRigidStatic* floor;
    RenderItem* floorRenderItem;



    bool moveForward = false;
    bool moveRight = false;
    bool moveLeft = false;

    double forwardForcePerSecond = 2000000;
    double torqueForcePerSecond =  20000000;

    double angle = 0;
    double anglePerSecond = 100;
    double minAngle = -60;
    double maxAngle = 60;


};
