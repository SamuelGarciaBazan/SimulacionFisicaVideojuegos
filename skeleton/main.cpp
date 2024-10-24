#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

std::string display_text = "This is a test";

#include "Vector3D.h"
#include "Particle.h"
#include "ParticleSystem.h"

using namespace physx;




class Axis {

private:

	std::vector <PxShape*> shapes;
	std::vector <PxTransform*> transforms;
	std::vector <PxVec4> colors;
	std::vector <RenderItem*> renderItems;

	int nSpheres = 4;

public:

	Axis(int radiusAxis = 10,int radiusPoints = 1) {


		shapes.reserve(nSpheres);
		transforms.reserve(nSpheres);
		renderItems.reserve(nSpheres);

		for (int i = 0; i < nSpheres; i++) {			
			shapes.push_back(CreateShape(PxSphereGeometry(radiusPoints)));
		}

		Vector3D zero(0,0,0);
		Vector3D x(radiusAxis,0,0);
		Vector3D y(0,radiusAxis,0);
		Vector3D z(0,0,radiusAxis);


		transforms.push_back(new PxTransform(PxVec3(zero.x, zero.y, zero.z)));
		transforms.push_back(new PxTransform(PxVec3(x.x, x.y, x.z)));
		transforms.push_back(new PxTransform(PxVec3(y.x, y.y, y.z)));
		transforms.push_back(new PxTransform(PxVec3(z.x, z.y, z.z)));

		colors.push_back(PxVec4(1,1,1,1));
		colors.push_back(PxVec4(1,0,0,1));
		colors.push_back(PxVec4(0,1,0,1));
		colors.push_back(PxVec4(0,0,1,1));

		for (int i = 0; i < nSpheres; i++) {

			renderItems.push_back(new RenderItem(shapes[i], transforms[i], colors[i]));
		}

	}

	~Axis() {

		for (int i = 0; i < nSpheres; i++) {

			DeregisterRenderItem(renderItems[i]);
		}

	}	
};

Axis* axis;
Particle* p;
std::vector<Particle*> particles;

std::vector<ParticleSystem*> particlesSystems;

ParticleSystem* particleSystemSnow;
Particle* snowModel;

ParticleSystem* particleSystemRain;
Particle* rainModel;

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


//forward declaration
void createSnowSystem();
void createRainSystem();



// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	//physX visual debugger creation
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;


	gScene = gPhysics->createScene(sceneDesc);


	axis = new Axis(20,2);
	p = new Particle(PxVec3(0, 50, 0),PxQuat(), PxVec3(250, 0, 0), PxVec3(0, 0, 0), 1, PxGeometryType::Enum::eSPHERE);


	p->scaleObject(250, 0.180, 0.1);



	createSnowSystem();
	//createRainSystem();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	//no compilador queja 
	PX_UNUSED(interactive);

	p->integrate(t);

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

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case 'C':
	{
		auto a = new Particle(PxVec3(0, 10, 0),PxQuat(), PxVec3(250, 0, 0), PxVec3(0, 0, 0), 1, PxGeometryType::Enum::eSPHERE);

		a->scaleObject(250, 0.180, 0.1);
		a->setFromCamera();

		particles.push_back(a);

		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
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



void createSnowSystem() {


	particleSystemSnow = new ParticleSystem(MyRandom::RandomMode::UNIFORM);

	particleSystemSnow->currentCreationTimer = 0;
	particleSystemSnow->creationRate = 0.1f;
	particleSystemSnow->acelMinRange = { 0,0,0 };
	particleSystemSnow->acelMaxRange = { 0,0,0 };

	particleSystemSnow->lifePosMinRange = { -50,0,-50 };
	particleSystemSnow->lifePosMaxRange = { 50,50,50 };

	particleSystemSnow->maxParticles = 1000;
	particleSystemSnow->startLifeTimeMinRange = 5.0f;
	particleSystemSnow->startLifeTimeMaxRange = 8.0f;

	particleSystemSnow->startPosMinRange = { -50,50,-50 };
	particleSystemSnow->startPosMaxRange = { 50,50,50 };

	particleSystemSnow->velMinRange = { -1,-10,-1 };
	particleSystemSnow->velMaxRange = { 1,-10,1 };

	particleSystemSnow->minScale = 1;
	particleSystemSnow->maxScale = 5;

	particleSystemSnow->diePos = true;
	particleSystemSnow->dieTime = false;


	PxQuat quat = PxQuat(0, 0, 0, 1);

	snowModel = new Particle(PxVec3(0, 30, 0),quat, PxVec3(250, 0, 0), PxVec3(0, 0, 0), 1,1,1, PxGeometryType::Enum::eSPHERE);


	particleSystemSnow->model = snowModel;

	particlesSystems.push_back(particleSystemSnow);
}

void createRainSystem() {


	particleSystemRain = new ParticleSystem(MyRandom::RandomMode::UNIFORM);

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


	PxQuat quat = PxQuat(0, 0, 0, 1);


	float angleRadians = -PxPiDivTwo;  
	PxVec3 rotationAxis(0.0f, 0.0f, 1.0f);  

	// Crear cuaterni�n a partir del �ngulo y el eje
	PxQuat rotationQuat(angleRadians, rotationAxis);

	quat = quat * rotationQuat;


	rainModel = new Particle(PxVec3(0, 30, 0), quat,PxVec3(250, 0, 0), PxVec3(0, 0, 0),
		1, 1, 1, PxGeometryType::Enum::eCAPSULE,PxVec4(0,0,1,1));



	particleSystemRain->model = rainModel;

	particlesSystems.push_back(particleSystemRain);
}