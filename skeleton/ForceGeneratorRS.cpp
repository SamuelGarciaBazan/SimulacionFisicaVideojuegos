#include "ForceGeneratorRS.h"
#include <iostream>


ForceGeneratorRS::ForceGeneratorRS(RigidSolid* afectedParticle, bool check)
	:checkIndependent(check)
{
	//solo lista local
	afectedParticlesLocal =  std::list<RigidSolid*>();
	afectedParticlesLocal.push_back(afectedParticle);

	//el resto null
	//afectedSystems = std::list<ParticleSystem*>();
	afectedParticlesRef = nullptr;
}

ForceGeneratorRS::ForceGeneratorRS(std::list<RigidSolid*>& afectedParticles, bool check)
	:checkIndependent(check)
{
	//solo lista referencia
	this->afectedParticlesRef = &afectedParticles;

	//el resto null
	//afectedSystems = std::list<ParticleSystem*>();
	afectedParticlesLocal = std::list<RigidSolid*>();
}

//ForceGeneratorRS::ForceGeneratorRS(ParticleSystem* particleSystem, bool check)
//	:checkIndependent(check)
//{
//	//solo lista sistemas
//	afectedSystems = std::list<ParticleSystem*>();
//	afectedSystems.push_back(particleSystem);
//
//	//el resto null
//	afectedParticlesRef = nullptr;
//	afectedParticlesLocal = std::list<Particle*>();
//}



//ForceGeneratorRS::ForceGeneratorRS(std::list<ParticleSystem*>& particleSystems, bool check)
//	:checkIndependent(check)
//{
//	//solo lista de sistemas
//	afectedSystems = particleSystems;
//
//	//el resto null
//	afectedParticlesRef = nullptr;
//	afectedParticlesLocal = std::list<Particle*>();
//}

void ForceGeneratorRS::update()
{
	if (!checkIndependent) {

		if (afectedParticlesRef != nullptr) {
			for (auto e : *afectedParticlesRef) {
				updateParticle(e);
			}
		}

		for (auto e : afectedParticlesLocal) {
			updateParticle(e);
		}

		//for (auto s : afectedSystems) {
		//	for (auto d : s->getParticlesData()) {
		//		auto e = d.particle;
		//		updateParticle(e);
		//	}
		//}
	}
	else{ // if else para evitar comparaciones innecesarias

		if (afectedParticlesRef != nullptr) {
			for (auto e : *afectedParticlesRef) {
				if(!e->getForceIndependent())
					updateParticle(e);
			}
		}

		for (auto e : afectedParticlesLocal) {
			if (!e->getForceIndependent())
				updateParticle(e);
		}

		//for (auto s : afectedSystems) {
		//	for (auto d : s->getParticlesData()) {
		//		auto e = d.particle;
		//		if (!e->getForceIndependent())
		//			updateParticle(e);
		//	}
		//}
	}
	
}

void ForceGeneratorRS::updateParticle(RigidSolid* target)
{
	if (afectCondition(target))
		target->getPxRigidDynamic()->addForce(forceCalculation(target));
}
