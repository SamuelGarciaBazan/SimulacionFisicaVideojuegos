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
	//gScene->addActor(*floor);


	//creacion del barco
	ship = new RigidSolid(allRigidSolids, gPhysics, gScene, { 0,0,0 }, { 3,3,8 }, { 0,1,0,1 },0.15,PxGeometryType::eBOX);

	ship->getPxRigidDynamic()->setMaxAngularVelocity(0.6);
	ship->getPxRigidDynamic()->setLinearDamping(0.75);
	ship->getPxRigidDynamic()->setAngularDamping(0.6);


	//creacion del sistema de flotacion
	bouyancyFGRS = new BouyancyForceGeneratorRS(allRigidSolids);

	//creacion del viento
	windFGRS = new WindForceGeneratorRS(allRigidSolids);

	windFGRS->setMinRange({ -100000,-100000,-100000 });
	windFGRS->setMaxRange({ 100000,100000,100000 });
	windFGRS->setK1(60);
	//windFGRS->setVelocity({10,0,0});


	windForceGenerator = new WindForceGenerator(allParticles,true);

	windForceGenerator->setMinRange({ -100000,-100000,-100000 });
	windForceGenerator->setMaxRange({ 100000,100000,100000 });
	windForceGenerator->setVelocity({ 50,0,0 });
	//creacion de la lluvia

	createRainSystem();

	createSnowSystem();
	//gravedad
	gravityForceGenerator = new GravityForceGenerator(allParticles,true);

	tornadoGen = new TornadoForceGenerator(particleSystemSnow);
	tornadoGen->setMinRange({ -250,-50,-250 });
	tornadoGen->setMaxRange({ -150,100,-150 });
}

ShipControlScene::~ShipControlScene()
{
	delete ship;
	DeregisterRenderItem(floorRenderItem);

	delete bouyancyFGRS;
	delete windFGRS;
}

void ShipControlScene::update(double t)
{
	std::cout << t << std::endl;
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
	PxVec3 torqueForceVector = shipTorqueVector * forwardForce * sin(angleRadians) ; //provisional por rozamiento

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

	// Calcula el eje de rotación usando el producto cruzado
	PxVec3 rotationAxis = forwardVector.cross(direction).getNormalized();

	// Calcula el ángulo entre los vectores usando el producto escalar
	float dotProduct = forwardVector.dot(direction);
	float angle = std::acos(dotProduct); // Ángulo en radianes
	// Construye el quaternion
	PxQuat rotation(angle, rotationAxis);

	// Aplica la rotación al cuerpo rígido
	PxTransform currentPose = newBullet->getPxRigidDynamic()->getGlobalPose();
	currentPose.q = rotation * currentPose.q; // Combina con la rotación actual
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

	// Crear cuaternión a partir del ángulo y el eje
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

	snowModel = new Particle(allParticles, PxVec3(0, 30, 0), quat, PxVec3(250, 0, 0), 1, 1, 1, PxGeometryType::Enum::eSPHERE,{0.4,0.4,0.4,1});
	snowModel->setForceIndependent(true);

	particleSystemSnow->model = snowModel;

	particlesSystems.push_back(particleSystemSnow);
}



