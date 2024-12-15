#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>


#include "ParticleSystem.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "TornadoForceGenerator.h"

#include "Axis.h"


#include "Scene.h"
#include "StaticSpringScene.h"
#include "DynamicSpringScene.h"
#include "DynamicSpringChainScene.h"
#include "ExplosionScene.h"
#include "WindScene.h"
#include "ElasticBandScene.h"
#include "BouyancyScene.h"


#include "ExampleSolidRigidScene.h"

#include "ShipControlScene.h"



std::string display_text = "This is a test";

using namespace physx;



#pragma region Practicas variables

//lista global de todas las particulas de la escena
std::list<Particle*> allParticles;

Scene* currentScene;

//ejes
Axis* axis;
Particle* p;

//lista de las particulas que se disparan por teclado
std::vector<Particle*> particles;


//lista de sistemas de particulas
std::vector<ParticleSystem*> particlesSystems;

//sistemas de particulas y sus modelos

ParticleSystem* particleSystemSnow;
Particle* snowModel;

ParticleSystem* particleSystemRain;
Particle* rainModel;

ParticleSystem* particleSystemWaterJet;
Particle* waterJetModel;


//generadores de fuerzas genericos
GravityForceGenerator* gravityGen = nullptr;
WindForceGenerator* windGen = nullptr;
TornadoForceGenerator* tornadoGen = nullptr;

#pragma endregion

#pragma region Application variables


//clase(interfaz) necesaria para el almacenamiento de memoria de physX, 
//esta clase viene con la SDK para facilitar el uso rapido de la API
//se puede crear otra a gusto del usuario
PxDefaultAllocator		gAllocator;

//clase(interfaz) necesaria para la gestion y manejo de errores de physX, 
//esta clase viene con la SDK para facilitar el uso rapido de la API
//se puede crear otra a gusto del usuario
PxDefaultErrorCallback	gErrorCallback;

//Every PhysX module requires a PxFoundation instance to be available
PxFoundation*			gFoundation = NULL;

PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;



#pragma endregion

PxCooking* gCooking = NULL;

//forward declaration

void createSnowSystem();
void createRainSystem();
void createWaterJetSystem();


// Initialize physics engine
void initPhysics(bool interactive)
{
	//INICIALIZACION DEL MOTOR

	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	//physX visual debugger creation
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);


	//para mallas
	PxCookingParams cookingParams(gPhysics->getTolerancesScale());
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, cookingParams);



	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;

	//CREACION DE LA ESCENA
	gScene = gPhysics->createScene(sceneDesc);

	//axis = new Axis(20,2);

	currentScene = new ShipControlScene(gPhysics, gScene);
	//currentScene = new BouyancyScene();
	//currentScene = new WindScene();


	//p = new Particle(allParticles, PxVec3(0, 50, 0),PxQuat(0,0,0,1), PxVec3(0, 0, 0), 1, PxGeometryType::Enum::eSPHERE);
	//p->scaleObject(250, 0.180, 0.1);


	//gravityGen = new GravityForceGenerator(allParticles); 
	//windGen = new WindForceGenerator(allParticles);
	//tornadoGen = new TornadoForceGenerator(allParticles);


	//windGen->setVelocity({ -20,0,0 });
	//windGen->setMinRange({ -50,-0,-50 });
	//windGen->setMaxRange({  50,50, 50 });

	///tornadoGen->setVelocity({ -20,0,0 });
	//tornadoGen->setMinRange({ -50,-0,-50 });
	//tornadoGen->setMaxRange({ 50,100, 50 });


	//createSnowSystem();
	//createRainSystem();
	//createWaterJetSystem();

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	//no compilador queja 
	PX_UNUSED(interactive);

	if (p != nullptr) {
		p->integrate(t);
	}

	//gravityGen->update();
	//windGen->update();
	//tornadoGen->update();

	currentScene->update(t);
	

	for (auto& sys : particlesSystems) sys->update(t);
	for (auto& par : particles) par->integrate(t);

	gScene->simulate(t);
	gScene->fetchResults(true);

}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	delete axis;
	delete p;
	delete gravityGen;

	delete currentScene;


	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	

}



// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	//std::cout << "KeyDown: " << key << std::endl;

	currentScene->keyPressed(key, camera);
}

void keyboardUp(unsigned char key, const PxTransform& camera) {
	PX_UNUSED(camera);
	//std::cout << "KeyUp: " << key << std::endl;

	currentScene->keyboardUp(key, camera);
}
void specialKeyDown(int key, const PxTransform& camera) {
	PX_UNUSED(camera);	
	//std::cout << "SpecialDown: " << key << std::endl;

	currentScene->specialKeyDown(key, camera);
}
void specialKeyUp(int key, const PxTransform& camera) {
	PX_UNUSED(camera);
	//std::cout << "SpecialUp: " << key << std::endl;

	currentScene->specialKeyUp(key, camera);
}

void joystickInput(unsigned int buttonMask, int x, int y, int z, const PxTransform& camera) {
	PX_UNUSED(camera);


	//std::cout << "Joystick: " << buttonMask << std::endl;
	//std::cout << "x: " << x << "y: "<< y << "z: " <<  buttonMask << std::endl;


	currentScene->joystickInput(buttonMask,x,y,z,camera);
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	currentScene->onCollision(actor1, actor2);

}

int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;

}


#pragma region Create particles systems

void createSnowSystem() {


	particleSystemSnow = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemSnow->currentCreationTimer = 0;
	particleSystemSnow->creationRate = 0.01f;
	particleSystemSnow->acelMinRange = { 0,0,0 };
	particleSystemSnow->acelMaxRange = { 0,0,0 };

	particleSystemSnow->lifePosMinRange = { -50,0,-50 };
	particleSystemSnow->lifePosMaxRange = { 50,50,50 };

	particleSystemSnow->maxParticles = 1000;
	particleSystemSnow->startLifeTimeMinRange = 5.0f;
	particleSystemSnow->startLifeTimeMaxRange = 8.0f;

	particleSystemSnow->startPosMinRange = { -50,50,-50 };
	particleSystemSnow->startPosMaxRange = { 50,50,50 };

	//particleSystemSnow->velMinRange = { -1,-10,-1 };
	//particleSystemSnow->velMaxRange = { 1,-10,1 };

	particleSystemSnow->velMinRange = { 0,0,0 };
	particleSystemSnow->velMaxRange = { 0,0,0 };



	particleSystemSnow->minScale = 1;
	particleSystemSnow->maxScale = 5;

	particleSystemSnow->diePos = false;
	particleSystemSnow->dieTime = false;

	particleSystemSnow->transform = PxTransform();

	particleSystemSnow->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemSnow->transform.q = physx::PxQuat(0, 0, 0, 1);

	PxQuat quat = PxQuat(0, 0, 0, 1);

	snowModel = new Particle(allParticles, PxVec3(0, 30, 0),quat, PxVec3(250, 0, 0), 1,1,1, PxGeometryType::Enum::eSPHERE);


	particleSystemSnow->model = snowModel;

	particlesSystems.push_back(particleSystemSnow);
}

void createRainSystem() {


	particleSystemRain = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemRain->currentCreationTimer = 0;
	particleSystemRain->creationRate = 0.01f;
	particleSystemRain->acelMinRange = { 0,0,0 };
	particleSystemRain->acelMaxRange = { 0,0,0 };

	particleSystemRain->lifePosMinRange = { -50,0,-50 };
	particleSystemRain->lifePosMaxRange = { 50,50,50 };

	particleSystemRain->maxParticles = 10000;
	particleSystemRain->startLifeTimeMinRange = 4.0f;
	particleSystemRain->startLifeTimeMaxRange = 6.0f;

	particleSystemRain->startPosMinRange = { -100,50,-100 };
	particleSystemRain->startPosMaxRange = { 100,50,100 };

	particleSystemRain->velMinRange = { 0,-60,-0 };
	particleSystemRain->velMaxRange = { 0,-200,-0 };

	particleSystemRain->minScale = 2;
	particleSystemRain->maxScale = 3;

	particleSystemRain->diePos = false;
	particleSystemRain->dieTime = true;

	particleSystemRain->transform = PxTransform();

	particleSystemRain->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemRain->transform.q = physx::PxQuat(0, 0, 0, 1);

	PxQuat quat = PxQuat(0, 0, 0, 1);


	float angleRadians = -PxPiDivTwo;  
	PxVec3 rotationAxis(0.0f, 0.0f, 1.0f);  

	// Crear cuaternión a partir del ángulo y el eje
	PxQuat rotationQuat(angleRadians, rotationAxis);

	quat = quat * rotationQuat;


	rainModel = new Particle(allParticles, PxVec3(0, 30, 0), quat,PxVec3(250, 0, 0), 
		1, 1, 1, PxGeometryType::Enum::eCAPSULE,PxVec4(0,0,1,1));



	particleSystemRain->model = rainModel;

	particlesSystems.push_back(particleSystemRain);
}

void createWaterJetSystem() {


	particleSystemWaterJet = new ParticleSystem(allParticles, MyRandom::RandomMode::UNIFORM);

	particleSystemWaterJet->currentCreationTimer = 0;
	particleSystemWaterJet->creationRate = 0.05f;
	particleSystemWaterJet->acelMinRange = { 0,0,0 };
	particleSystemWaterJet->acelMaxRange = { 0,0,0 };

	//particleSystemWaterJet->lifePosMinRange = { -50,0,-50 };
	//particleSystemWaterJet->lifePosMaxRange = { 50,50,50 };

	particleSystemWaterJet->maxParticles = 10000;
	particleSystemWaterJet->startLifeTimeMinRange = 4.0f;
	particleSystemWaterJet->startLifeTimeMaxRange = 6.0f;

	particleSystemWaterJet->startPosMinRange = { -2,0,-2 };
	particleSystemWaterJet->startPosMaxRange = { 2,0,2 };

	particleSystemWaterJet->velMinRange = { -5,60,-5 };
	particleSystemWaterJet->velMaxRange = { 5,80,5 };

	particleSystemWaterJet->minScale = 2;
	particleSystemWaterJet->maxScale = 3;

	particleSystemWaterJet->diePos = false;
	particleSystemWaterJet->dieTime = true;

	particleSystemWaterJet->transform = PxTransform();

	particleSystemWaterJet->transform.p = physx::PxVec3(0, 0, 0);
	particleSystemWaterJet->transform.q = physx::PxQuat(0, 0, 0, 1);

	float angleRadians = -PxPiDivTwo;
	PxVec3 rotationAxis(0.0f, 0.0f, 1.0f);

	// Crear cuaternión a partir del ángulo y el eje
	PxQuat rotationQuat(angleRadians, rotationAxis);

	particleSystemWaterJet->transform.q = particleSystemWaterJet->transform.q * rotationQuat;


	PxQuat quat = PxQuat(0, 0, 0, 1);

	waterJetModel = new Particle(allParticles, PxVec3(0, 30, 0), quat, PxVec3(250, 0, 0),
		1, 1, 1, PxGeometryType::Enum::eSPHERE, PxVec4(0, 0, 1, 1));


	particleSystemWaterJet->model = waterJetModel;

	particlesSystems.push_back(particleSystemWaterJet);
}

#pragma endregion

