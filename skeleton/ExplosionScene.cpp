#include "ExplosionScene.h"


using namespace physx;

ExplosionScene::ExplosionScene()
{
	createSnowSystem();
	tornadoGen = new TornadoForceGenerator(allParticles);
	tornadoGen->setMinRange({ -50,-0,-50 });
	tornadoGen->setMaxRange({ 50,100, 50 });
}

ExplosionScene::~ExplosionScene()
{
}

void ExplosionScene::update(double t)
{
	tornadoGen->update();
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

void ExplosionScene::createSnowSystem()
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

	particleSystemSnow->velMinRange = { 0,0,0 };
	particleSystemSnow->velMaxRange = { 0,0,0 };



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

