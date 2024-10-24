#pragma once

#include <list>
#include <random>
#include "Particle.h"

#include <random>


class MyRandom {

public:
	MyRandom() noexcept {};
	~MyRandom() noexcept{};

	enum RandomMode {
		UNIFORM, NORMAL
	};

	RandomMode getRandomMode() const noexcept{
		return randomMode;
	}

	void setRandomMode(RandomMode mode) noexcept{
		randomMode = mode;
	}

	double getRandomRange(double min, double max) {
		
		//si el rango no es valido lo swapeamos
		if(min > max){
			std::swap(min, max);
		}

		if (randomMode == UNIFORM) {
			
			uniformParam._Min = min;
			uniformParam._Max = max;
		
			uniformDistribution.param(uniformParam);
			
			return uniformDistribution(gen);
		}
		else if (randomMode == NORMAL) {

			normalParam._Mean =  (min+max)/2;//media 
			normalParam._Sigma = (max-min)/6;//desviacion tipica 99.7% de los valores

			normalDistribution.param(normalParam);

			return normalDistribution(gen);
		}
		return 0;
	}


private:

	RandomMode randomMode;

	std::random_device randomDevice{};
	std::mt19937 gen{ randomDevice()};


	std::uniform_real_distribution<> uniformDistribution;
	std::normal_distribution<> normalDistribution;

	std::uniform_real_distribution<>::param_type uniformParam;
	std::normal_distribution<>::param_type normalParam;


};

class ParticleSystem
{
public:

	ParticleSystem() { random.setRandomMode(MyRandom::RandomMode::UNIFORM); };
	~ParticleSystem() {};

	void update(double d);

	int maxParticles;

	double creationRate;
	double currentCreationTimer;

	double startLifeTimeMinRange;
	double startLifeTimeMaxRange;

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
	Particle* model;


private:

	struct ParticleData
	{
		//la posicion la sacamos de la particula
		Particle* particle;
		//tiempo de vida de la particula
		double currentLifeTime;
	};

	//lista de particulas
	std::list<ParticleData> particles;

	


	MyRandom random;

	void deleteParticles(double d);

	bool mustDie(ParticleData p);

	void createNewParticles(double d);

	void createParticle();


	physx::PxVec3 getRandomVec3(physx::PxVec3 min, physx::PxVec3 max);


};

