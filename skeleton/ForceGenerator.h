#pragma once 

#include "Particle.h"
#include "ParticleSystem.h"



//Clase abstracta que representa un generador de fuerzas
class ForceGenerator {

public:
	//constructores con Particula / lista de particulas
	ForceGenerator(Particle* afectedParticle, bool check = false);
	ForceGenerator(std::list<Particle*>& afectedParticles,bool check = false);

	//constructores con Psystem / lista Psystems
	ForceGenerator(ParticleSystem* particleSystem, bool check = false);
	ForceGenerator(std::list<ParticleSystem*>& particleSystemList, bool check = false);

	//destructor virtual
	virtual ~ForceGenerator() = 0;

	//recorre todos los elementos afectados y les aplica la fuerza
	void update();


	//get/set checkIndependent
	inline bool getCheckIndependent() noexcept {
		return checkIndependent;
	}
	inline void setCheckIndependent(bool b) noexcept {
		checkIndependent = b;
	}

protected:

	//este metodo es el que se sobreescribe en las clases hijas, con el calculo adecuado para cada sistema
	virtual double forceCalculation(Particle* target) = 0;

	//listas de elementos afectados 

	std::list<Particle*> afectedParticlesLocal; //lista de particulas local
	std::list<Particle*>* afectedParticlesRef;//la lista de particulas referencia
	std::list<ParticleSystem*> afectedSystems;//la lista de sistemas es una copia

	//si es true, no afecta a las particulas independientes
	//si es false, afecta a todas las particulas sin mirar si son independientes o no
	bool checkIndependent = false; //por defecto no chequea
};