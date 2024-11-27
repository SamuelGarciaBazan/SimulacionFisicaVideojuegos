#include "BouyancyScene.h"

BouyancyScene::BouyancyScene()
{

	afectedParticle = new Particle(
		allParticles,
		{10,10,10 }, //pos
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
		{ 20,0.01,20 }, //scale
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


	windGen = new WindForceGenerator(allParticles, true);
	windGen->setMinRange({ -1000, -1000, -1000 });
	windGen->setMaxRange({1000, 1000, 1000});

}

BouyancyScene::~BouyancyScene()
{
}

void BouyancyScene::update(double t)
{
	for (auto e : allParticles) e->integrate(t);
	bouyancyGenerator->update();
	gravityGen->update();
	windGen->update();
}

void BouyancyScene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
}
