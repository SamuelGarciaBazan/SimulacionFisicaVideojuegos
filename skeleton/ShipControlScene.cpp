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
	updateMoveForward(t);
	updateMoveLeftRight(t);

	



}

void ShipControlScene::specialKeyDown(int key, const physx::PxTransform& camera)
{

	
	switch (key)
	{
	case GLUT_KEY_UP: 
		moveForward = true;
		break;	
	case GLUT_KEY_RIGHT: 
		moveRight = true;
		break;
	case GLUT_KEY_LEFT: 
		moveLeft = true;
		break;	
	default:
		break;
	}

}

void ShipControlScene::specialKeyUp(int key, const physx::PxTransform& camera)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		moveForward = false;
		break;
	case GLUT_KEY_RIGHT:
		moveRight = false;
		break;
	case GLUT_KEY_LEFT:
		moveLeft = false;
		break;
	default:
		break;
	}
}

void ShipControlScene::updateMoveForward(double t)
{
	double forwardForce = 0;

	if (moveForward) forwardForce += forwardForcePerSecond * t;

	//obtener el transform del barco
	PxTransform shipTransform = ship->getPxRigidDynamic()->getGlobalPose();

	PxVec3 shipForwardVector = shipTransform.q.rotate(PxVec3(0, 0, -1)); // Eje Z en espacio local

	PxVec3 forwardForceVector = shipForwardVector * forwardForce;

	// Aplicar la fuerza al centro de masa
	ship->getPxRigidDynamic()->addForce(forwardForceVector);

	
	//std::cout << "se ha aplicado una fuerza forward de :" << forwardForce << std::endl;
}

void ShipControlScene::updateMoveLeftRight(double t)
{
	double torqueForce = 0;

	if (moveRight) torqueForce += torqueForcePerSecond * t;
	if (moveLeft) torqueForce -= torqueForcePerSecond * t;

	//obtener el transform del barco
	PxTransform shipTransform = ship->getPxRigidDynamic()->getGlobalPose();

	PxVec3 shipTorqueVector = shipTransform.q.rotate(PxVec3(0, 1, 0));
	
	PxVec3 shipTorqueForce = shipTorqueVector * torqueForce ;
	
	ship->getPxRigidDynamic()->addTorque(shipTorqueForce);

	//std::cout << "se ha aplicado una fuerza torque de :" << torqueForce << std::endl;

	auto v = ship->getPxRigidDynamic()->getLinearVelocity();

	std::cout << "Linear Velocity [ x:" << v.x << " y: " << v.y << " z: " << v.z << " ]" << std::endl;

}


