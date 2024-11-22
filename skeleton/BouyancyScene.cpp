#include "BouyancyScene.h"

BouyancyScene::BouyancyScene()
{

	afectedParticle = new Particle(
		allParticles,
		{ 0,30,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		{ 1,1,1 }, //scale
		1, //damping
		1,//mass
		physx::PxGeometryType::eBOX,
		{ 1,0,0,1 } // color
	);



	liquidParticle = new Particle(
		allParticles,
		{ 0,0,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		{10,0.01,10}, //scale
		1, //damping
		1,//mass
		physx::PxGeometryType::eBOX,
		{ 0,1,1,1 } // color
	);


	afectedParticle->setForceIndependent(false);
	liquidParticle->setForceIndependent(true);

	bouyancyGenerator = new BouyancyForceGenerator(afectedParticle);

	bouyancyGenerator->setLiquidParticle(liquidParticle);

	gravityGen = new GravityForceGenerator(allParticles);

	gravityGen->setCheckIndependent(true);

}

BouyancyScene::~BouyancyScene()
{
}

void BouyancyScene::update(double t)
{
	for (auto e : allParticles) e->integrate(t);
	bouyancyGenerator->update();
	gravityGen->update();
}

void BouyancyScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
}
