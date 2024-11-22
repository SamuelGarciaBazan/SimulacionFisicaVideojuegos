#include "ElasticBandScene.h"

ElasticBandScene::ElasticBandScene()
{
	dynamicSpringParticleA = new Particle(
		allParticles,
		{ 0,15,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		1, //scale
		1, //damping
		1,//mass
		physx::PxGeometryType::eSPHERE,
		{ 1,0,0,1 } // color
	);


	dynamicSpringParticleB = new Particle(
		allParticles,
		{ 0,60,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		1, //scale
		1, //damping
		1,//mass
		physx::PxGeometryType::eSPHERE,
		{ 0,0,1,1 } // color
	);

	dynamicSpringGenA = new ElasticBandForceGenerator(dynamicSpringParticleA, dynamicSpringParticleB);

	dynamicSpringGenA->setK(1); //para diff entre integracion semi/implc, 5000
	dynamicSpringGenA->setReposeLenght(100);

	dynamicSpringGenB = new ElasticBandForceGenerator(dynamicSpringParticleB, dynamicSpringParticleA);

	dynamicSpringGenB->setK(1); //para diff entre integracion semi/implc, 5000
	dynamicSpringGenB->setReposeLenght(100);
}

ElasticBandScene::~ElasticBandScene()
{
}

void ElasticBandScene::update(double t)
{
	dynamicSpringGenA->update();
	dynamicSpringGenB->update();

	dynamicSpringParticleA->integrate(t);
	dynamicSpringParticleB->integrate(t);
}

void ElasticBandScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
}
