#include "StaticSpringScene.h"



StaticSpringScene::StaticSpringScene() 
{
	staticSpringParticle = new Particle(
		allParticles,
		{ 5,15,0 }, //pos
		{ 0,0,0,1 }, //rot
		{ 0,0,0 }, // vel
		1, //scale
		1, //damping
		1 //mass
	);

	staticSpringGen = new SpringForceGenerator(staticSpringParticle);

	staticSpringGen->setPos({ 0,0,0 });
	staticSpringGen->setK(50); //para diff entre integracion semi/implc, 5000
	staticSpringGen->setReposeLenght(10);
}

StaticSpringScene::~StaticSpringScene()
{
}

void StaticSpringScene::update(double t)
{
	staticSpringParticle->integrate(t);
	staticSpringGen->update();
}
