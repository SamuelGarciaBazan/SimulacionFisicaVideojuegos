#pragma once
#include "Scene.h"

#include "RigidSolid.h"

#include "BouyancyForceGeneratorRS.h"
#include "WindForceGeneratorRS.h"

#include "WindForceGenerator.h"
#include "TornadoForceGenerator.h"
#include "GravityForceGenerator.h"

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


    std::string getTimonValue() {
        char buffer[32]; // Búfer temporal
        sprintf(buffer, "%.2f", angle); // Escribe el resultado
        std::string text(buffer); // Asigna al string
        return text;
    }

private:

    enum Side {
        LEFT,
        RIGHT
    };


    void updateMove(double t);

    void updateShooting(double t);

    void shootBullet(Side side);

    RigidSolid* createBullet(int type,physx::PxVec3 startPoint);

    void deleteBullets(double t);

    void rotateBullet(RigidSolid* newBullet, physx::PxVec3 direction);


    void createRainSystem();

    void createSnowSystem();

    physx::PxPhysics* gPhysics = nullptr;
    physx::PxScene* gScene = nullptr;

    RigidSolid* ship = nullptr;
    RigidSolid* islote = nullptr;

    physx::PxRigidStatic* floor = nullptr;

    physx::PxRigidStatic* cubo1 = nullptr;
    physx::PxRigidStatic* cubo2 = nullptr;
    physx::PxRigidStatic* cubo3 = nullptr;
    physx::PxRigidStatic* cubo4 = nullptr;


    RenderItem* floorRenderItem = nullptr;

    RenderItem* cubo1RenderItem = nullptr;
    RenderItem* cubo2RenderItem = nullptr;
    RenderItem* cubo3RenderItem = nullptr;
    RenderItem* cubo4RenderItem = nullptr;


    BouyancyForceGeneratorRS* bouyancyFGRS = nullptr;
    WindForceGeneratorRS* windFGRS = nullptr;

    WindForceGenerator* windForceGenerator = nullptr;
    GravityForceGenerator* gravityForceGenerator = nullptr;

    TornadoForceGenerator* tornadoGen = nullptr;

    bool moveForward = false;
    bool moveRight = false;
    bool moveLeft = false;

    double forwardForcePerSecond = 15000000;
    double torqueForcePerSecond =  20000000;

    double angle = 0;
    double anglePerSecond = 30;
    double minAngle = -45;
    double maxAngle = 45;


    //variables para disparo de proyectiles

    struct BulletInfo {

        RigidSolid* bullet;
        double aliveTime = 0.0f;
    };

    double bulletMaxTime = 7.0f;

    std::list<BulletInfo*> bullets;
    
    double fireRate = .5f; //varios fire rates(por ammo?)

    bool shootingLeft = false;
    bool shootingRight = false;

    double fireCounterLeft = 0.0f;
    double fireCounterRight = 0.0f;

    int ammoType = 0; //0 bola cañon , 1 astilla grande cuadrada

    double bulletSpawnPointOffset = 10;
    double bulletImpulseForceType0 = 1000;
    double bulletImpulseForceType1 = 300;


    //para sistema de particulas

    ParticleSystem* particleSystemRain;
    Particle* rainModel;



    ParticleSystem* particleSystemSnow;
    Particle* snowModel;
};

