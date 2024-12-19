#pragma once


#include <list>
#include <random>
#include "ParticleSystem.h"

#include <random>

#include "RigidSolid.h"


class ParticleSystemRB
{
private:

	struct RigidSolidData
	{
		//la posicion la sacamos de la particula
		RigidSolid* particle;
		//tiempo de vida de la particula
		double currentLifeTime;
	};
public:

	struct ParticleSystemCreateInfo {

		int maxParticles;

		double creationRate;
		double currentCreationTimer;

		double startLifeTimeMinRange;
		double startLifeTimeMaxRange;

		double minScale;
		double maxScale;

		physx::PxVec3 lifePosMinRange;
		physx::PxVec3 lifePosMaxRange;

		physx::PxVec3 velMinRange;
		physx::PxVec3 velMaxRange;

		physx::PxVec3 acelMinRange;
		physx::PxVec3 acelMaxRange;

		physx::PxVec3 startPosMinRange;
		physx::PxVec3 startPosMaxRange;


		bool loop;
		double duration;
		double delay;

		bool diePos;
		bool dieTime;


		//modelo de particula
		RigidSolid* model;

		MyRandom::RandomMode randomMode;

	};

	struct RigidSolidModelData {
		physx::PxVec3 scale;
		physx::PxVec4 color;
		double density;
		physx::PxGeometryType::Enum type;
		bool forceIndependent;
	};


	ParticleSystemRB( std::list<RigidSolid*>& allParticles,
		physx::PxPhysics* gPhysics,
		physx::PxScene* gScene,
		MyRandom::RandomMode randomMode
	) : randomMode(randomMode),
		allParticles(allParticles),
		gPhysics(gPhysics),
		gScene(gScene)
	
	{

		random.setRandomMode(randomMode);

	};
	~ParticleSystemRB() {};

	void update(double d);

	int maxParticles;

	double creationRate;
	double currentCreationTimer;

	double startLifeTimeMinRange;
	double startLifeTimeMaxRange;

	double minScale;
	double maxScale;

	physx::PxVec3 lifePosMinRange;
	physx::PxVec3 lifePosMaxRange;

	physx::PxVec3 velMinRange;
	physx::PxVec3 velMaxRange;

	physx::PxVec3 acelMinRange;
	physx::PxVec3 acelMaxRange;

	physx::PxVec3 startPosMinRange;
	physx::PxVec3 startPosMaxRange;


	bool loop;
	double duration;
	double delay;

	bool diePos;
	bool dieTime;


	//modelo de particula
	RigidSolidModelData* model;

	MyRandom::RandomMode randomMode;

	physx::PxTransform transform;

	std::list<RigidSolidData> const& getParticlesData() {
		return particles;
	}

private:

	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;


	//lista de particulas
	std::list<RigidSolidData> particles;




	MyRandom random;

	void deleteParticles(double d);

	bool mustDie(RigidSolidData p);

	void createNewParticles(double d);

	void createParticle();


	physx::PxVec3 getRandomVec3(physx::PxVec3 min, physx::PxVec3 max);


	std::list<RigidSolid*>& allParticles;

};

