#include "ShipControlScene.h"

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
	floorRenderItem = new RenderItem(floorShape, floor, { 1,0.5,0.5,1 });

	//ship = 


}

ShipControlScene::~ShipControlScene()
{
}

void ShipControlScene::update(double t)
{
}

void ShipControlScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
}
