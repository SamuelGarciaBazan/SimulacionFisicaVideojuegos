#pragma once 

#include "Particle.h"
#include "ParticleSystem.h"


/*
EXPLICACION SISTEMAS DE FUERZAS

Los generadores, reciben una particula/lista o un sistema/sistemas de particulas

Cada clase que herede de ForceGenerator debera implementar los metodos:
	-forceCalculation, donde estara la logica de generacion de fuerzas de cada generador
	-afectCondition, donde estara la logica de la condicion que debe cumplir la particula para que sea afectada 
	por ejemplo un area

Las particulas tienen un bool forceIndependent
Los generadores tienen un bool checkIndependent

En funcion estos booleanos, aplicaran o no la fuerza que tengan que aplicar sobre la particula
Con diferentes combinaciones podemos conseguir diferentes efectos: 
	Sistemas de particulas independientes a otros generadores,generadores que afecten a todo, generadores
	que afecten a todo menos lo independiente ...


*/



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
	virtual ~ForceGenerator() {};

	//recorre todos los elementos afectados y les aplica la fuerza
	void update();


	//get/set checkIndependent
	inline bool getCheckIndependent() const noexcept {
		return checkIndependent;
	}
	inline void setCheckIndependent(bool b) noexcept {
		checkIndependent = b;
	}

private:

	void updateParticle(Particle* target);


protected:

	//este metodo es el que se sobreescribe en las clases hijas, con el calculo adecuado para cada sistema
	virtual physx::PxVec3 forceCalculation(Particle* target) = 0;

	//este metodo es el que se sobreescribe en las clases hijas, con el calculo adecuado para cada sistema
	virtual bool afectCondition(Particle* target) = 0;

	//listas de elementos afectados 

	std::list<Particle*> afectedParticlesLocal; //lista de particulas local
	std::list<Particle*>* afectedParticlesRef;//la lista de particulas referencia
	std::list<ParticleSystem*> afectedSystems;//la lista de sistemas es una copia

	//si es true, no afecta a las particulas independientes
	//si es false, afecta a todas las particulas sin mirar si son independientes o no
	bool checkIndependent = false; //por defecto no chequea
};