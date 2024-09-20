#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

std::string display_text = "This is a test";

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

		transforms.push_back(new PxTransform(PxVec3(0, 0, 0)));
		transforms.push_back(new PxTransform(PxVec3(radiusAxis, 0, 0)));
		transforms.push_back(new PxTransform(PxVec3(0, radiusAxis, 0)));
		transforms.push_back(new PxTransform(PxVec3(0, 0, radiusAxis)));

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


PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;



// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

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

	axis = new Axis(20,5);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	delete axis;


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
	case ' ':
	{
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


