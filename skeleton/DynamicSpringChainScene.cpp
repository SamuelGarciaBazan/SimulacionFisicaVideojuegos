#include "DynamicSpringChainScene.h"

DynamicSpringChainScene::DynamicSpringChainScene()
{
	int n = 5;

	dynamicSpringGenChains.clear();
	dynamicSpringParticlesChain.clear();

	float posOffset = 6;

	for (int i = 0; i < n; i++) {

		Particle* p = new Particle(
			allParticles,
			{ 0,i * posOffset,0 }, //pos
			{ 0,0,0,1 }, //rot
			{ 0,0,0 }, // vel
			1, //scale
			1, //damping
			1,//mass
			physx::PxGeometryType::eSPHERE,
			{ 1,0,0,1 } // color
		);

		dynamicSpringParticlesChain.emplace_back(p);
	}

	for (int i = 0; i < n - 1; i++) {


		dynamicSpringGenChains.emplace_back(
			SpringForceGenerator(
				dynamicSpringParticlesChain[i],
				dynamicSpringParticlesChain[i + 1]));

		dynamicSpringGenChains[i].setK(5); //para diff entre integracion semi/implc, 5000
		dynamicSpringGenChains[i].setReposeLenght(5);

	}
}

DynamicSpringChainScene::~DynamicSpringChainScene()
{
}

void DynamicSpringChainScene::update(double t)
{
	for (auto e : dynamicSpringGenChains) e.update();
	for (auto e : dynamicSpringParticlesChain) e->integrate(t);
}
