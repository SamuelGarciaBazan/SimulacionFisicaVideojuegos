#include "RigidSolid.h"


using namespace physx;

RigidSolid::RigidSolid(
	physx::PxPhysics* gPhysics, 
	physx::PxScene* gScene,
	physx::PxVec3 pos,
	physx::PxVec3 scale,
	double density,
	physx::PxGeometryType::Enum type) :
	gPhysics(gPhysics), gScene(gScene)
{
	//geometry y shape
	PxGeometry* geo;
	PxShape* shape;

	//creacion de la geometria dependiendo del tipo
	switch (type)
	{
	case physx::PxGeometryType::eSPHERE:
		geo = new PxSphereGeometry(scale.x);
		break;
	case physx::PxGeometryType::eCAPSULE:
		geo = new PxCapsuleGeometry(scale.x, 2 * scale.y);
		break;
	case physx::PxGeometryType::eBOX:
		geo = new PxBoxGeometry(scale.x, scale.y, scale.z);
		break;

	default:
		geo = new PxSphereGeometry(1);
		break;
	}

	//creacion del shape
	shape = CreateShape(*geo);


	rigidDynamic = gPhysics->createRigidDynamic({PxTransform{pos.x,pos.y,pos.z}});

	rigidDynamic->attachShape(*shape);

	//importante para la distribucion de masas y calculo de momentos de inercia
	PxRigidBodyExt::updateMassAndInertia(*rigidDynamic, density);

	gScene->addActor(*rigidDynamic);
	renderItem = new RenderItem(shape, rigidDynamic, { 0,1,0,1 });
}

RigidSolid::~RigidSolid()
{

	DeregisterRenderItem(renderItem);
}
