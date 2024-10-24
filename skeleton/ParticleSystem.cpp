#include "ParticleSystem.h"

using namespace std;
using namespace physx;


void ParticleSystem::update(double d)
{
	for (auto& pData : particles) {
		pData.particle->integrate(d);
	}


	//eliminar particulas correspondientes
	//actualizar vida de las particulas y  ver si han salido del area
	deleteParticles(d);

	//crear particulas si se puede (tiempo y cantidad)
	createNewParticles(d);
}

#pragma region Delete Particles

void ParticleSystem::deleteParticles(double d)
{
	auto it = particles.begin();

	//recorrido de particulas
	while (it != particles.end()) {

		auto& p = (*it);

		//actualizamos el tiempo de vida
		p.currentLifeTime -= d;

		//si debe morir, la eliminamos
		if (mustDie(p)) {
			delete p.particle;
			it = particles.erase(it);
		}
		else ++it;
	}

}

bool ParticleSystem::mustDie(ParticleData p)
{
	//muerte por tiempo
	if (dieTime && p.currentLifeTime < 0) {
		return true;
	}
	else {

		//muerte por posicion
		if (diePos) {

			const PxVec3 pos = p.particle->getPos();

			if (pos.x < lifePosMinRange.x || pos.x > lifePosMaxRange.x ||
				pos.y < lifePosMinRange.y || pos.y > lifePosMaxRange.y ||
				pos.z < lifePosMinRange.z || pos.z > lifePosMaxRange.z) {
				return true;
			}
		}
		
		//ninguna muerte
		return false;
	}

}


#pragma endregion


#pragma region Create particles

void ParticleSystem::createNewParticles(double d)
{
	//actualizar el timer
	currentCreationTimer += d;

	//crear la cantidad de particulas que haga falta
	while (currentCreationTimer > creationRate) {
		currentCreationTimer -= creationRate;

		createParticle();
	}
}

void ParticleSystem::createParticle()
{
	//valores aleatorios en los rangos
	PxVec3 pos = getRandomVec3(startPosMinRange, startPosMaxRange);
	PxVec3 vel = getRandomVec3(velMinRange, velMaxRange);
	PxVec3 acel = getRandomVec3(acelMinRange, acelMaxRange);

	double startLifetime = random.getRandomRange(startLifeTimeMinRange, startLifeTimeMaxRange);
	double scale = random.getRandomRange(minScale, maxScale);

	//valores leidos del modelo
	double damping = model->getDamping();
	double mass = model->getMass();
	PxGeometryType::Enum type = model->getGeometryType();
	PxVec4 color = model->getColor();
	PxQuat quat = model->getQuat();

	//aplicar el transform del particle system a la nueva particula
	pos += transform.p;
	vel = transform.q.rotate(vel);
	acel = transform.q.rotate(acel);

	Particle* newParticle = new Particle(pos,quat,vel,acel,scale,damping,mass,type,color);

	ParticleData data;
	data.particle = newParticle;
	data.currentLifeTime = startLifetime;


	particles.push_back(data);
}



#pragma endregion

physx::PxVec3 ParticleSystem::getRandomVec3(physx::PxVec3 min, physx::PxVec3 max)
{
	return physx::PxVec3(random.getRandomRange(min.x, max.x),
		random.getRandomRange(min.y, max.y),
		random.getRandomRange(min.z, max.z));
}


