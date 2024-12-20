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
	PxShape* floorShape = CreateShape(PxBoxGeometry(10000, 0.1, 10000));
	floor->attachShape(*floorShape);

	floorRenderItem = new RenderItem(floorShape, floor, { 0,0.15,0.7,1 });

	//no lo a�adimos a la escena para que no colisione
	//gScene->addActor(*floor);

	float boxSize = 600;

	//----------------------------------------------------

	cubo1 = gPhysics->createRigidStatic({ PxTransform{0,0,-boxSize} });
	PxShape* cubo1shape = CreateShape(PxBoxGeometry(boxSize, 10, 10));
	cubo1->attachShape(*cubo1shape);

	cubo1RenderItem = new RenderItem(cubo1shape, cubo1, { 1,0,0,1 });
	gScene->addActor(*cubo1);

	//----------------------------------------------------

	cubo2 = gPhysics->createRigidStatic({ PxTransform{0,0,boxSize} });
	PxShape* cubo2shape = CreateShape(PxBoxGeometry(boxSize, 10, 10));
	cubo2->attachShape(*cubo2shape);

	cubo2RenderItem = new RenderItem(cubo2shape, cubo2, { 1,0,0,1 });
	gScene->addActor(*cubo2);

	//----------------------------------------------------


	cubo3 = gPhysics->createRigidStatic({ PxTransform{boxSize,0,0} });
	PxShape* cubo3shape = CreateShape(PxBoxGeometry(10, 10, boxSize));
	cubo3->attachShape(*cubo3shape);

	cubo3RenderItem = new RenderItem(cubo3shape, cubo3, { 1,0,0,1 });
	gScene->addActor(*cubo3);


	//----------------------------------------------------


	cubo4 = gPhysics->createRigidStatic({ PxTransform{-boxSize,0,0} });
	PxShape* cubo4shape = CreateShape(PxBoxGeometry(10, 10, boxSize));
	cubo4->attachShape(*cubo4shape);

	cubo4RenderItem = new RenderItem(cubo4shape, cubo4, { 1,0,0,1 });
	gScene->addActor(*cubo4);


	//----------------------------------------------------





	//----------------------------------------------------
	//creacion del barco
	ship = new RigidSolid(allRigidSolids, gPhysics, gScene, { 0,0,0 }, { 4,12,10 }, { 0,1,0,1 },0.15,PxGeometryType::eBOX);

	ship->getPxRigidDynamic()->setMaxAngularVelocity(0.7);
	ship->getPxRigidDynamic()->setLinearDamping(0.75);
	ship->getPxRigidDynamic()->setAngularDamping(0.6);


	ship->getPxRigidDynamic()->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
	ship->getPxRigidDynamic()->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
	
	//----------------------------------------------------


	//----------------------------------------------------

	//creacion del sistema de flotacion
	bouyancyFGRS = new BouyancyForceGeneratorRS(allRigidSolids);

	////creacion del viento
	//windFGRS = new WindForceGeneratorRS(allRigidSolids);

	//windFGRS->setMinRange({ -100000,-100000,-100000 });
	//windFGRS->setMaxRange({ 100000,100000,100000 });
	//windFGRS->setK1(60);
	//windFGRS->setVelocity({10,0,0});

	//----------------------------------------------------


	//----------------------------------------------------


	windForceGenerator = new WindForceGenerator(allParticles,true);

	windForceGenerator->setMinRange({ -100000,-100000,-100000 });
	windForceGenerator->setMaxRange({ 100000,100000,100000 });
	windForceGenerator->setVelocity({ 50,0,0 });
	//creacion de la lluvia


	//----------------------------------------------------

	createRainSystem();

	createSnowSystem();
	//gravedad
	gravityForceGenerator = new GravityForceGenerator(allParticles,true);

	tornadoGen = new TornadoForceGenerator(particleSystemSnow);
	tornadoGen->setMinRange({ -250,-50,-250 });
	tornadoGen->setMaxRange({ -150,100,-150 });



	islote = new RigidSolid(allRigidSolids, gPhysics, gScene, { 50,0,-50 }, { 8,8,8 }, { 1,0,0,1 }, 0.15, PxGeometryType::eSPHERE);


	//creacion del sistema de meteoritos

	meteoritosSystem = new ParticleSystemRB(allRigidSolids, gPhysics, gScene, MyRandom::RandomMode::UNIFORM);

	ParticleSystemRB::RigidSolidModelData* model = new ParticleSystemRB::RigidSolidModelData();

	model->color = { 1,0,0,1 };
	model->density = 0.15;
	model->forceIndependent = false;
	model->scale = { 5,5,5 };
	model->type = physx::PxGeometryType::Enum::eSPHERE;
	model->linearDamping = 0.7;
	model->angularDamping = 0.0;



	meteoritosSystem->model = model;

	meteoritosSystem->startPosMinRange = {-100,20,-100};
	meteoritosSystem->startPosMaxRange = {100,20,100};
		
	meteoritosSystem->creationRate = 3.f;
	meteoritosSystem->startLifeTimeMinRange = 20.f;
	meteoritosSystem->startLifeTimeMaxRange = 40.f;
	meteoritosSystem->diePos = false;
	meteoritosSystem->dieTime = true;

	meteoritosSystem->transform = PxTransform();

	meteoritosSystem->transform.p = physx::PxVec3(0, 0, 0);
	meteoritosSystem->transform.q = physx::PxQuat(0, 0, 0, 1);

}

ShipControlScene::~ShipControlScene()
{
	delete ship;
	delete islote;
	DeregisterRenderItem(floorRenderItem);

	delete bouyancyFGRS;
	delete windFGRS;

	delete windForceGenerator;
	delete gravityForceGenerator;
	delete tornadoGen;

	gScene->removeActor(*cubo1);
	gScene->removeActor(*cubo2);
	gScene->removeActor(*cubo3);
	gScene->removeActor(*cubo4);
	cubo1->release();
	cubo2->release();
	cubo3->release();
	cubo4->release();


	DeregisterRenderItem(cubo1RenderItem);
	DeregisterRenderItem(cubo2RenderItem);
	DeregisterRenderItem(cubo3RenderItem);
	DeregisterRenderItem(cubo4RenderItem);

	delete meteoritosSystem->model;
	delete meteoritosSystem;

}

void ShipControlScene::update(double t)
{
	//std::cout << t << std::endl;
	
	
	updateMove(t);
	updateShooting(t);

	deleteBullets(t);

	bouyancyFGRS->update();
	//windFGRS->update();

	gravityForceGenerator->update();
	windForceGenerator->update();

	particleSystemRain->update(t);

	particleSystemSnow->update(t);
	tornadoGen->update();

	meteoritosSystem->update(t);


	//auto v = ship->getPxRigidDynamic()->getAngularVelocity();
	//std::cout << "angVelocity [ x:" << v.x << " y: " << v.y << " z: " << v.z << " ]" << std::endl;

}

void ShipControlScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	switch (key)
	{
	case '1':
		shootingLeft = true;
		break;
	case '2':
		shootingRight = true;
		break;
	case '3':
		ammoType = (ammoType + 1) % 2; //swap entre tipos de municion
		break;
	default:
		break;
	}

}

void ShipControlScene::keyboardUp(unsigned char key, const physx::PxTransform& camera)
{
	switch (key)
	{
	case '1':
		shootingLeft = false;
		break;
	case '2':
		shootingRight = false;
		break;
	
	default:
		break;
	}
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
	PxVec3 torqueForceVector = shipTorqueVector * forwardForce * sin(angleRadians) ;

	// Aplicar la fuerza al centro de masa
	ship->getPxRigidDynamic()->addForce(forwardForceVector);
	ship->getPxRigidDynamic()->addTorque(torqueForceVector);

	//debug
	//std::cout << "Angle: " << angle << std::endl;
	//std::cout << "forwardForce [ x:" << forwardForceVector.x << " y: " << forwardForceVector.y << " z: " << forwardForceVector.z << " ]" << std::endl;
	//std::cout << "torqueForce [ x:" << torqueForceVector.x << " y: " << torqueForceVector.y << " z: " << torqueForceVector.z << " ]" << std::endl;
}

void ShipControlScene::updateShooting(double t)
{
	fireCounterLeft += t;
	fireCounterRight += t;

	if (shootingLeft && fireCounterLeft > fireRate) {

		fireCounterLeft = 0;
		shootBullet(Side::LEFT);
	}
	if (shootingRight && fireCounterRight > fireRate) {

		fireCounterRight = 0;
		shootBullet(Side::RIGHT);
	}


}

void ShipControlScene::shootBullet(Side side)
{

	PxTransform shipTransform = ship->getPxRigidDynamic()->getGlobalPose();

	PxVec3 startPoint = shipTransform.p;

	PxVec3 localDir = side == Side::LEFT ? PxVec3(2, 1, 0) : PxVec3(-2, 1, 0); //direccion inicial bala

	PxVec3 direction = shipTransform.q.rotate(localDir).getNormalized();

	startPoint += direction * bulletSpawnPointOffset;

	RigidSolid* newBullet = createBullet(ammoType, startPoint);



	//para que apunten bien

	if (ammoType == 1) {

		rotateBullet(newBullet, direction);

		if (side == Side::LEFT) {
			auto v = newBullet->getPxRigidDynamic()->getGlobalPose().q.rotate(PxVec3{ 0,0,-1 }).getNormalized();
			//newBullet->getPxRigidDynamic()->addTorque(v*20,PxForceMode::eIMPULSE);
		}
		else {
			auto v = newBullet->getPxRigidDynamic()->getGlobalPose().q.rotate(PxVec3{ 0,0,1 }).getNormalized();

			//newBullet->getPxRigidDynamic()->addTorque(v * 20, PxForceMode::eIMPULSE);
		}


	}




	PxVec3 force = direction * (ammoType == 0 ? bulletImpulseForceType0 : bulletImpulseForceType1);

	newBullet->getPxRigidDynamic()->addForce(force, PxForceMode::eIMPULSE);

	bullets.push_back(new BulletInfo{ newBullet,0 });


	//debug
	//std::cout << "shoot" << std::endl;
	//std::cout << "force [ x:" << force.x << " y: " << force.y << " z: " << force.z << " ]" << std::endl;
	//std::cout << "Mass: " << newBullet->getPxRigidDynamic()->getMass() << std::endl;
}

RigidSolid* ShipControlScene::createBullet(int type, physx::PxVec3 startPoint)
{
	RigidSolid* newBullet;
	
	switch (type)
	{
	case 0: {
		newBullet = new RigidSolid(allRigidSolids, gPhysics, gScene,
			startPoint,
			{ 3,3,3 },//scale
			{ 0.3,0.3,0.3,1 }, //color
			0.4); //desity

		break;
	}
	case 1: {

		newBullet = new RigidSolid(allRigidSolids, gPhysics, gScene,
			startPoint,
			{ 3,1,1 },//scale
			{ 1,0,0,1 }, //color
			0.4,physx::PxGeometryType::eBOX); //desity

		break;
	}
	default:
		break;
	}


	return newBullet;
}

void ShipControlScene::deleteBullets(double t)
{
	auto it = bullets.begin();

	//recorrido de particulas
	while (it != bullets.end()) {

		auto& p = (*it);

		//actualizamos el tiempo de vida
		p->aliveTime += t;

		//si debe morir, la eliminamos
		if (p->aliveTime > bulletMaxTime) {
			delete p->bullet;
			it = bullets.erase(it);
		}
		else ++it;
	}

}

void ShipControlScene::rotateBullet(RigidSolid* newBullet, PxVec3 direction)
{
	PxVec3 forwardVector(1, 0, 0);

	// Calcula el eje de rotaci�n usando el producto cruzado
	PxVec3 rotationAxis = forwardVector.cross(direction).getNormalized();

	// Calcula el �ngulo entre los vectores usando el producto escalar
	float dotProduct = forwardVector.dot(direction);
	float angle = std::acos(dotProduct); // �ngulo en radianes
	// Construye el quaternion
	PxQuat rotation(angle, rotationAxis);

	// Aplica la rotaci�n al cuerpo r�gido
	PxTransform currentPose = newBullet->getPxRigidDynamic()->getGlobalPose();
	currentPose.q = rotation * currentPose.q; // Combina con la rotaci�n actual
	newBullet->getPxRigidDynamic()->setGlobalPose(currentPose);
}

void ShipControlScene::createRainSystem()
{
	particleSystemRain = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemRain->currentCreationTimer = 0;
	particleSystemRain->creationRate = 0.05f;
	particleSystemRain->acelMinRange = { 0,0,0 };
	particleSystemRain->acelMaxRange = { 0,0,0 };

	particleSystemRain->lifePosMinRange = { -50,0,-50 };
	particleSystemRain->lifePosMaxRange = { 50,50,50 };

	particleSystemRain->maxParticles = 50;
	particleSystemRain->startLifeTimeMinRange = 4.0f;
	particleSystemRain->startLifeTimeMaxRange = 6.0f;

	particleSystemRain->startPosMinRange = { -300,100,-300 };
	particleSystemRain->startPosMaxRange = {300,100,300 };

	particleSystemRain->velMinRange = { 0,-30,-0 };
	particleSystemRain->velMaxRange = { 0,-80,-0 };

	particleSystemRain->minScale = 1;
	particleSystemRain->maxScale = 2.5;

	particleSystemRain->diePos = false;
	particleSystemRain->dieTime = true;

	particleSystemRain->transform = PxTransform();

	particleSystemRain->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemRain->transform.q = physx::PxQuat(0, 0, 0, 1);

	PxQuat quat = PxQuat(0, 0, 0, 1);


	float angleRadians = -PxPiDivTwo;
	PxVec3 rotationAxis(0.0f, 0.0f, 1.0f);

	// Crear cuaterni�n a partir del �ngulo y el eje
	PxQuat rotationQuat(angleRadians, rotationAxis);

	quat = quat * rotationQuat;


	rainModel = new Particle(allParticles, PxVec3(0, 30000, 0), quat, PxVec3(250, 0, 0),
		1, 1, 4, PxGeometryType::Enum::eCAPSULE, PxVec4(0, 0, 1, 1));



	particleSystemRain->model = rainModel;



	particlesSystems.push_back(particleSystemRain);
}

void ShipControlScene::createSnowSystem()
{
	particleSystemSnow = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemSnow->currentCreationTimer = 0;
	particleSystemSnow->creationRate = 0.1f;
	particleSystemSnow->acelMinRange = { 0,0,0 };
	particleSystemSnow->acelMaxRange = { 0,0,0 };

	//particleSystemSnow->lifePosMinRange = { -50,0,-50 };
	//particleSystemSnow->lifePosMaxRange = { 50,50,50 };

	particleSystemSnow->maxParticles = 1000;
	particleSystemSnow->startLifeTimeMinRange = 5.0f;
	particleSystemSnow->startLifeTimeMaxRange = 10.0f;

	particleSystemSnow->startPosMinRange = { -250,0,-250 };
	particleSystemSnow->startPosMaxRange = { -150,0,-150 };

	//particleSystemSnow->velMinRange = { -1,-10,-1 };
	//particleSystemSnow->velMaxRange = { 1,-10,1 };

	particleSystemSnow->velMinRange = { 0,0,0 };
	particleSystemSnow->velMaxRange = { 0,0,0 };



	particleSystemSnow->minScale = 5;
	particleSystemSnow->maxScale = 9;

	particleSystemSnow->diePos = false;
	particleSystemSnow->dieTime = true;

	particleSystemSnow->transform = PxTransform();

	particleSystemSnow->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemSnow->transform.q = physx::PxQuat(0, 0, 0, 1);

	PxQuat quat = PxQuat(0, 0, 0, 1);

	snowModel = new Particle(allParticles, PxVec3(0, 30000, 0), quat, PxVec3(250, 0, 0), 1, 1, 1, PxGeometryType::Enum::eSPHERE,{0.4,0.4,0.4,1});
	snowModel->setForceIndependent(true);

	particleSystemSnow->model = snowModel;

	particlesSystems.push_back(particleSystemSnow);
}



