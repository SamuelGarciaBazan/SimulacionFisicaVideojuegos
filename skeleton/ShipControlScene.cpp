#include "ShipControlScene.h"
#include <iostream>
using namespace physx;

ShipControlScene::ShipControlScene(
	physx::PxPhysics* gPhysics,
	physx::PxScene* gScene) :
	gPhysics(gPhysics), gScene(gScene)
{
	floor = gPhysics->createRigidStatic({ PxTransform{0,0,0} });
	PxShape* floorShape = CreateShape(PxBoxGeometry(100, 0.1, 100));

	floor->attachShape(*floorShape);


	gScene->addActor(*floor);
	floorRenderItem = new RenderItem(floorShape, floor, { 0,0.15,0.7,1 });

	ship = new RigidSolid(gPhysics, gScene, { 0,10,0 }, { 3,3,8 }, { 0,1,0,1 },0.15,PxGeometryType::eBOX);



}

ShipControlScene::~ShipControlScene()
{
}

void ShipControlScene::update(double t)
{

}

void ShipControlScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	switch (toupper(key))
	{
	case '1':
	{
		//ship->getPxRigidDynamic()->addForce({ 0,1000,0 });
		//std::cout << "aaa" << std::endl;

		break;
	}
	default:
		break;
	}
}
