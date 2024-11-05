#include "ForceGenerator.h"



ForceGenerator::ForceGenerator(Particle* afectedParticle, bool check)
	:checkIndependent(check)
{
	//solo lista local
	afectedParticlesLocal =  std::list<Particle*>();
	afectedParticlesLocal.push_back(afectedParticle);

	//el resto null
	afectedSystems = std::list<ParticleSystem*>();
	afectedParticlesRef = nullptr;
}

ForceGenerator::ForceGenerator(std::list<Particle*>& afectedParticles, bool check)
	:checkIndependent(check)
{
	//solo lista referencia
	this->afectedParticlesRef = &afectedParticles;

	//el resto null
	afectedSystems = std::list<ParticleSystem*>();
	afectedParticlesLocal = std::list<Particle*>();
}

ForceGenerator::ForceGenerator(ParticleSystem* particleSystem, bool check)
	:checkIndependent(check)
{
	//solo lista sistemas
	afectedSystems = std::list<ParticleSystem*>();
	afectedSystems.push_back(particleSystem);

	//el resto null
	afectedParticlesRef = nullptr;
	afectedParticlesLocal = std::list<Particle*>();
}



ForceGenerator::ForceGenerator(std::list<ParticleSystem*>& particleSystems, bool check)
	:checkIndependent(check)
{
	//solo lista de sistemas
	afectedSystems = particleSystems;

	//el resto null
	afectedParticlesRef = nullptr;
	afectedParticlesLocal = std::list<Particle*>();
}

void ForceGenerator::update()
{
	if (!checkIndependent) {
		if (afectedParticlesRef != nullptr) {
			for (auto e : *afectedParticlesRef) {
				e->addForce(forceCalculation(e));
			}
		}

		for (auto e : afectedParticlesLocal) {
			e->addForce(forceCalculation(e));

		}

		for (auto s : afectedSystems) {
			for (auto d : s->getParticlesData()) {
				auto e = d.particle;
				e->addForce(forceCalculation(e));
			}
		}
	}
	else{ // if else para evitar comparaciones innecesarias

		if (afectedParticlesRef != nullptr) {
			for (auto e : *afectedParticlesRef) {
				if(!e->getForceIndependent())
					e->addForce(forceCalculation(e));
			}
		}

		for (auto e : afectedParticlesLocal) {
			if (!e->getForceIndependent())
				e->addForce(forceCalculation(e));

		}

		for (auto s : afectedSystems) {
			for (auto d : s->getParticlesData()) {
				auto e = d.particle;
				if (!e->getForceIndependent())
					e->addForce(forceCalculation(e));
			}
		}
	}
	
}
