#include "WindScene.h"


using namespace physx;

WindScene::WindScene()
{
	windGen = new WindForceGenerator(allParticles);

	windGen->setVelocity({ -20,0,0 });
	windGen->setMinRange({ -500,-500,-500 });
	windGen->setMaxRange({ 500,500, 500 });

	createSnowSystem();
}

WindScene::~WindScene()
{
}

void WindScene::update(double t)
{
	windGen->update();
	for (auto e : particlesSystems) e->update(t);
}

void WindScene::createSnowSystem()
{

	particleSystemSnow = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemSnow->currentCreationTimer = 0;
	particleSystemSnow->creationRate = 0.01f;
	particleSystemSnow->acelMinRange = { 0,0,0 };
	particleSystemSnow->acelMaxRange = { 0,0,0 };

	particleSystemSnow->lifePosMinRange = { -50,0,-50 };
	particleSystemSnow->lifePosMaxRange = { 50,50,50 };

	particleSystemSnow->maxParticles = 1000;
	particleSystemSnow->startLifeTimeMinRange = 5.0f;
	particleSystemSnow->startLifeTimeMaxRange = 5.0f;

	particleSystemSnow->startPosMinRange = { -50,50,-50 };
	particleSystemSnow->startPosMaxRange = { 50,50,50 };

	//particleSystemSnow->velMinRange = { -1,-10,-1 };
	//particleSystemSnow->velMaxRange = { 1,-10,1 };

	particleSystemSnow->velMinRange = { 0,-40,0 };
	particleSystemSnow->velMaxRange = { 0,-70,0 };



	particleSystemSnow->minScale = 1;
	particleSystemSnow->maxScale = 5;

	particleSystemSnow->diePos = false;
	particleSystemSnow->dieTime = true;

	particleSystemSnow->transform = PxTransform();

	particleSystemSnow->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemSnow->transform.q = physx::PxQuat(0, 0, 0, 1);

	PxQuat quat = PxQuat(0, 0, 0, 1);

	snowModel = new Particle(allParticles, PxVec3(0, 30, 0), quat, PxVec3(250, 0, 0), 1, 1, 1, PxGeometryType::Enum::eSPHERE);


	particleSystemSnow->model = snowModel;

	particlesSystems.push_back(particleSystemSnow);
}
