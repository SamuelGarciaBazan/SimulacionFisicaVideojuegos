#include "DynamicSpringScene.h"

DynamicSpringScene::DynamicSpringScene()
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
		{ 0,30,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		1, //scale
		1, //damping
		1,//mass
		physx::PxGeometryType::eSPHERE,
		{ 0,0,1,1 } // color
	);

	dynamicSpringGenA = new SpringForceGenerator(dynamicSpringParticleA, dynamicSpringParticleB);

	dynamicSpringGenA->setK(5); //para diff entre integracion semi/implc, 5000
	dynamicSpringGenA->setReposeLenght(10);

	dynamicSpringGenB = new SpringForceGenerator(dynamicSpringParticleB, dynamicSpringParticleA);

	dynamicSpringGenB->setK(5); //para diff entre integracion semi/implc, 5000
	dynamicSpringGenB->setReposeLenght(10);
}

DynamicSpringScene::~DynamicSpringScene()
{
}

void DynamicSpringScene::update(double t)
{
	dynamicSpringGenA->update();
	dynamicSpringGenB->update();

	dynamicSpringParticleA->integrate(t);
	dynamicSpringParticleB->integrate(t);
}

