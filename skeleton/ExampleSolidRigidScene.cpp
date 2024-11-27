#include "ExampleSolidRigidScene.h"
#include <iostream>
#include <setjmp.h>

using namespace physx;


ExampleSolidRigidScene::ExampleSolidRigidScene(physx::PxPhysics* gPhysics, physx::PxScene* gScene)
	:gPhysics(gPhysics),gScene(gScene)
{

	//creacion del suelo

	PxRigidStatic* suelo = gPhysics->createRigidStatic({ PxTransform{0,0,0} });
	PxShape* floorShape = CreateShape(PxBoxGeometry(100, 0.1, 100));

	suelo->attachShape(*floorShape);

	
	gScene->addActor(*suelo);
	floorRenderItem = new RenderItem(floorShape, suelo, { 0.8,0.8,0.8,1 });



	//creacion del cubo 
	PxRigidDynamic* cube;
	cube = gPhysics->createRigidDynamic({ PxTransform{0,50,0} });
	cube->setLinearVelocity({ 0,5,0 });
	cube->setAngularVelocity({ 0,0,0 });

	PxVec3 size{ 5,5,5 };


	PxShape* cubeShape = CreateShape(PxBoxGeometry(5,5,5));
	cube->attachShape(*cubeShape);

	//importante para la distribucion de masas y calculo de momentos de inercia
	PxRigidBodyExt::updateMassAndInertia(*cube, 0.15);
	
	/*
	char inst[4 * 3] = {
		0x00, 0x01, 0x25, 0xFF,

	};
	int (*func)(void) = (int (*)(void))((void *) & inst);
	int result = func();
	*/
	
	
	//tensor de inercia manual
	//cube->setMassSpaceInertiaTensor({size.y * size.z,size.x * size.z,size.x*size.y});

	gScene->addActor(*cube);
	cubeRenderItem = new RenderItem(cubeShape, cube, { 0,1,0,1 });


}

ExampleSolidRigidScene::~ExampleSolidRigidScene()
{
	DeregisterRenderItem(floorRenderItem);

}

void ExampleSolidRigidScene::update(double t)
{
}

void ExampleSolidRigidScene::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{

	//std::cout << "aaaa desde la escena" << std::endl;


}
