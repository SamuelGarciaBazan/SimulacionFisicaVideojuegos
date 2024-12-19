#include "ParticleSystemRB.h"


using namespace std;
using namespace physx;


void ParticleSystemRB::update(double d)
{
	for (auto& pData : particles) {
		//pData.particle->integrate(d);
	}


	//eliminar particulas correspondientes
	//actualizar vida de las particulas y  ver si han salido del area
	deleteParticles(d);

	//crear particulas si se puede (tiempo y cantidad)
	createNewParticles(d);
}

#pragma region Delete Particles

void ParticleSystemRB::deleteParticles(double d)
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

bool ParticleSystemRB::mustDie(RigidSolidData p)
{
	//muerte por tiempo
	if (dieTime && p.currentLifeTime < 0) {
		return true;
	}
	else {

		//muerte por posicion
		if (diePos) {

			const PxVec3 pos = p.particle->getPxRigidDynamic()->getGlobalPose().p;

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

void ParticleSystemRB::createNewParticles(double d)
{
	//actualizar el timer
	currentCreationTimer += d;

	//crear la cantidad de particulas que haga falta
	while (currentCreationTimer > creationRate) {
		currentCreationTimer -= creationRate;

		createParticle();
	}
}

void ParticleSystemRB::createParticle()
{
	//valores aleatorios en los rangos
	PxVec3 pos = getRandomVec3(startPosMinRange, startPosMaxRange);
	PxVec3 vel = getRandomVec3(velMinRange, velMaxRange);
	PxVec3 acel = getRandomVec3(acelMinRange, acelMaxRange);

	double startLifetime = random.getRandomRange(startLifeTimeMinRange, startLifeTimeMaxRange);
	double scale = random.getRandomRange(minScale, maxScale);

	//valores leidos del modelo
	bool forceIndependent = model->forceIndependent;
	//aplicar el transform del particle system a la nueva particula
	pos += transform.p;
	vel = transform.q.rotate(vel);
	acel = transform.q.rotate(acel);


	RigidSolid* newParticle = new RigidSolid(
		allParticles,
		gPhysics,
		gScene,
		pos,
		model->scale,
		model->color, 
		model->density,
		model->type);

	newParticle->setForceIndependent(forceIndependent);

	RigidSolidData data;
	data.particle = newParticle;
	data.currentLifeTime = startLifetime;

	particles.push_back(data);
}



#pragma endregion

physx::PxVec3 ParticleSystemRB::getRandomVec3(physx::PxVec3 min, physx::PxVec3 max)
{
	return physx::PxVec3(random.getRandomRange(min.x, max.x),
		random.getRandomRange(min.y, max.y),
		random.getRandomRange(min.z, max.z));
}


