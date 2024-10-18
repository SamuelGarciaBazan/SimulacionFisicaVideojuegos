#include "ParticleSystem.h"

using namespace std;
using namespace physx;


void ParticleSystem::update(double d)
{

	//eliminar particulas correspondientes
	//actualizar vida de las particulas y  ver si han salido del area
	deleteParticles(d);

	//crear particulas si se puede (tiempo y cantidad)
	createNewParticles(d);
}


void ParticleSystem::deleteParticles(double d)
{
	auto it = particles.begin();

	while (it != particles.end()) {

		auto& p = (*it);

		p.currentLifeTime -= d;


		if (mustDie(p)) {
			delete p.particle;
			it = particles.erase(it);
		}
		else ++it;
	}

}

void ParticleSystem::createNewParticles(double d)
{
}

bool ParticleSystem::mustDie(ParticleData p)
{
	if (p.currentLifeTime < 0) {
		return true;
	}
	else {

		const PxVec3 pos = p.particle->getPos();

		if (pos.x < lifePosMinRange.x || pos.x > lifePosMaxRange.x ||
			pos.y < lifePosMinRange.y || pos.y > lifePosMaxRange.y ||
			pos.z < lifePosMinRange.z || pos.z > lifePosMaxRange.z) {
			return true;
		}
		return false;
	}

}
