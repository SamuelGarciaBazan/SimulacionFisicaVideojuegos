#include "ExplosionScene.h"


using namespace physx;

ExplosionScene::ExplosionScene()
{
	createWaterJetSystem();
}

ExplosionScene::~ExplosionScene()
{
}

void ExplosionScene::update(double t)
{
	for (auto e : particlesSystems) e->update(t);
	if (explosionGen != nullptr) {
		explosionGen->update();
		explosionGen->updateTime(t);
	}
}

void ExplosionScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	switch (toupper(key)) {

	case 'E':
	{
		//creacion de la explosion
		explosionGen = new ExplosionForceGenerator(allParticles);

		explosionGen->setRadius(1000);
		explosionGen->setCenter({ 0,50,0 });
		explosionGen->setK(500000);

		break;
	}

	default:
		break;
	}

}

void ExplosionScene::createWaterJetSystem()
{
	particleSystemWaterJet = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemWaterJet->currentCreationTimer = 0;
	particleSystemWaterJet->creationRate = 0.05f;
	particleSystemWaterJet->acelMinRange = { 0,0,0 };
	particleSystemWaterJet->acelMaxRange = { 0,0,0 };

	//particleSystemWaterJet->lifePosMinRange = { -50,0,-50 };
	//particleSystemWaterJet->lifePosMaxRange = { 50,50,50 };

	particleSystemWaterJet->maxParticles = 10000;
	particleSystemWaterJet->startLifeTimeMinRange = 4.0f;
	particleSystemWaterJet->startLifeTimeMaxRange = 6.0f;

	particleSystemWaterJet->startPosMinRange = { -2,0,-2 };
	particleSystemWaterJet->startPosMaxRange = { 2,0,2 };

	particleSystemWaterJet->velMinRange = { -5,60,-5 };
	particleSystemWaterJet->velMaxRange = { 5,80,5 };

	particleSystemWaterJet->minScale = 2;
	particleSystemWaterJet->maxScale = 3;

	particleSystemWaterJet->diePos = false;
	particleSystemWaterJet->dieTime = true;

	particleSystemWaterJet->transform = PxTransform();

	particleSystemWaterJet->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemWaterJet->transform.q = physx::PxQuat(0, 0, 0, 1);

	float angleRadians = -PxPiDivTwo;
	PxVec3 rotationAxis(0.0f, 0.0f, 1.0f);

	// Crear cuaternión a partir del ángulo y el eje
	PxQuat rotationQuat(angleRadians, rotationAxis);

	particleSystemWaterJet->transform.q = particleSystemWaterJet->transform.q * rotationQuat;


	PxQuat quat = PxQuat(0, 0, 0, 1);

	waterJetModel = new Particle(allParticles, PxVec3(0, 30, 0), quat, PxVec3(250, 0, 0),
		1, 1, 1, PxGeometryType::Enum::eSPHERE, PxVec4(0, 0, 1, 1));


	particleSystemWaterJet->model = waterJetModel;

	particlesSystems.push_back(particleSystemWaterJet);
}
