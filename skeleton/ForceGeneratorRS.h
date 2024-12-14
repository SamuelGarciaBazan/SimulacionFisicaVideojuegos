#pragma once
#pragma once 

#include "RigidSolid.h"



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
class ForceGeneratorRS {

public:
	//constructores con RigidSolid / lista RigidSolid
	ForceGeneratorRS(RigidSolid* afectedRigidSolid, bool check = false);
	ForceGeneratorRS(std::list<RigidSolid*>& afectedRigidSolids, bool check = false);

	//constructores con Psystem / lista Psystems
	//ForceGenerator(ParticleSystem* particleSystem, bool check = false);
	//ForceGenerator(std::list<ParticleSystem*>& particleSystemList, bool check = false);

	//destructor virtual
	virtual ~ForceGeneratorRS() {};

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

	void updateParticle(RigidSolid* target);


protected:

	//este metodo es el que se sobreescribe en las clases hijas, con el calculo adecuado para cada sistema
	virtual physx::PxVec3 forceCalculation(RigidSolid* target) = 0;

	//este metodo es el que se sobreescribe en las clases hijas, con el calculo adecuado para cada sistema
	virtual bool afectCondition(RigidSolid* target) = 0;

	//listas de elementos afectados 

	std::list<RigidSolid*> afectedParticlesLocal; //lista de particulas local
	std::list<RigidSolid*>* afectedParticlesRef;//la lista de particulas referencia
	//std::list<ParticleSystem*> afectedSystems;//la lista de sistemas es una copia

	//si es true, no afecta a las particulas independientes
	//si es false, afecta a todas las particulas sin mirar si son independientes o no
	bool checkIndependent = false; //por defecto no chequea
};


