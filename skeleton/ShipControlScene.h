#pragma once
#include "Scene.h"

#include "RigidSolid.h"

#include "BouyancyForceGeneratorRS.h"
#include "WindForceGeneratorRS.h"

class ShipControlScene :
    public Scene
{
public:

    ShipControlScene(physx::PxPhysics* gPhysics, physx::PxScene* gScene);
    virtual ~ShipControlScene();

    virtual void update(double t);

    virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);
    virtual void keyboardUp(unsigned char key, const physx::PxTransform& camera) ;

    virtual void specialKeyDown(int key, const physx::PxTransform& camera);
    virtual void specialKeyUp(int key, const physx::PxTransform& camera);

private:

    enum Side {
        LEFT,
        RIGHT
    };

    void updateMove(double t);

    void updateShooting(double t);

    void shootBullet(Side side);


    physx::PxPhysics* gPhysics = nullptr;
    physx::PxScene* gScene = nullptr;

    RigidSolid* ship = nullptr;

    physx::PxRigidStatic* floor = nullptr;
    RenderItem* floorRenderItem = nullptr;


    BouyancyForceGeneratorRS* bouyancyFGRS = nullptr;
    WindForceGeneratorRS* windFGRS = nullptr;



    bool moveForward = false;
    bool moveRight = false;
    bool moveLeft = false;

    double forwardForcePerSecond = 2000000;
    double torqueForcePerSecond =  20000000;

    double angle = 0;
    double anglePerSecond = 100;
    double minAngle = -45;
    double maxAngle = 45;


    //variables para disparo de proyectiles
    
    double fireRate = 1.5f; //varios fire rates(por ammo?)

    bool shootingLeft = false;
    bool shootingRight = false;

    double fireCounterLeft = 0.0f;
    double fireCounterRight = 0.0f;

    int ammoType = 0; //0 bola cañon , 1 astilla grande cuadrada

    double bulletSpawnPointOffset = 10;
    double bulletImpulseForce = 1000;

};

