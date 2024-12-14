#include "ShipControlScene.h"
#include <iostream>
#include "algorithm"


using namespace physx;

ShipControlScene::ShipControlScene(
	physx::PxPhysics* gPhysics,
	physx::PxScene* gScene) :
	gPhysics(gPhysics), gScene(gScene)
{

	//creacion del suelo
	floor = gPhysics->createRigidStatic({ PxTransform{0,0,0} });

	PxShape* floorShape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	floor->attachShape(*floorShape);

	floorRenderItem = new RenderItem(floorShape, floor, { 0,0.15,0.7,1 });
	//gScene->addActor(*floor);


	//creacion del barco
	ship = new RigidSolid(allRigidSolids, gPhysics, gScene, { 0,0,0 }, { 3,3,8 }, { 0,1,0,1 },0.15,PxGeometryType::eBOX);

	//ship->getPxRigidDynamic()->setMaxAngularVelocity(1);
	//ship->getPxRigidDynamic()->setLinearDamping(0.9);
	//ship->getPxRigidDynamic()->setAngularDamping(0.6);


	//creacion del sistema de flotacion
	bouyancyFGRS = new BouyancyForceGeneratorRS(ship);

}

ShipControlScene::~ShipControlScene()
{
	delete ship;
	DeregisterRenderItem(floorRenderItem);
}

void ShipControlScene::update(double t)
{
	updateMove(t);
	bouyancyFGRS->update();

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

void ShipControlScene::updateMove(double t)
{
	//actualizar el angulo
	if (moveRight) angle += anglePerSecond * t;
	if (moveLeft) angle -= anglePerSecond * t;

	//clampeamos para no poder girar demasiado
	angle = std::clamp(angle, minAngle, maxAngle);

	//ver la fuerza hacia delante
	double forwardForce = 0;

	if (moveForward) forwardForce += forwardForcePerSecond * t;

	//obtener el transform del barco
	PxTransform shipTransform = ship->getPxRigidDynamic()->getGlobalPose();

	//angulos de las fuerzas
	PxVec3 shipForwardVector = shipTransform.q.rotate(PxVec3(0, 0, -1)); // Eje -Z
	PxVec3 shipTorqueVector = shipTransform.q.rotate(PxVec3(0, 1, 0)); //Eje Y 

	//angulo en radianes
	double angleRadians = angle * std::_Pi_val / 180.0;

	//vectores de fuerza (dir * fuerza * sin||cos)
	PxVec3 forwardForceVector = shipForwardVector * forwardForce * cos(angleRadians);
	PxVec3 torqueForceVector = shipTorqueVector * forwardForce * sin(angleRadians) ; //provisional por rozamiento

	// Aplicar la fuerza al centro de masa
	ship->getPxRigidDynamic()->addForce(forwardForceVector);
	ship->getPxRigidDynamic()->addTorque(torqueForceVector);

	//debug
	std::cout << "Angle: " << angle << std::endl;
	std::cout << "forwardForce [ x:" << forwardForceVector.x << " y: " << forwardForceVector.y << " z: " << forwardForceVector.z << " ]" << std::endl;
	std::cout << "torqueForce [ x:" << torqueForceVector.x << " y: " << torqueForceVector.y << " z: " << torqueForceVector.z << " ]" << std::endl;
}



