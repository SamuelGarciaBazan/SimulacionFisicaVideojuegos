#include "ExampleSolidRigidScene.h"
#include <iostream>
#include <setjmp.h>

#include "RigidSolid.h"

#include<cmath>


using namespace physx;


ExampleSolidRigidScene::ExampleSolidRigidScene(physx::PxPhysics* gPhysics, physx::PxScene* gScene, physx::PxCooking* gCooking)
	:gPhysics(gPhysics),gScene(gScene), gCooking(gCooking)
{

	//creacion del suelo

	PxRigidStatic* suelo = gPhysics->createRigidStatic({ PxTransform{0,0,0} });
	PxShape* floorShape = CreateShape(PxBoxGeometry(100, 0.1, 100));

	suelo->attachShape(*floorShape);

	
	gScene->addActor(*suelo);
	floorRenderItem = new RenderItem(floorShape, suelo, { 0.8,0.8,0.8,1 });



	//creacion del cubo 
	PxRigidDynamic* cube;
	cube = gPhysics->createRigidDynamic({ PxTransform{0,50,0} });
	cube->setLinearVelocity({ 0,5,0 });
	cube->setAngularVelocity({ 0,0,0 });

	PxVec3 size{ 5,5,5 };


	PxShape* cubeShape = CreateShape(PxBoxGeometry(5,5,5));
	cube->attachShape(*cubeShape);

	//importante para la distribucion de masas y calculo de momentos de inercia
	PxRigidBodyExt::updateMassAndInertia(*cube, 0.15);

	float mass = 150;
	
	cube->setMass(150);
	
	//TESTING TENSORES DE INCERCIA
	float d = (2.0/3.0)*cube->getMass()* std::pow(5, 2);

	//cube->setMassSpaceInertiaTensor({ d,d,d });
	
	//tensor de inercia manual
	//cube->setMassSpaceInertiaTensor({size.y * size.z,size.x * size.z,size.x*size.y});

	gScene->addActor(*cube);
	cubeRenderItem = new RenderItem(cubeShape, cube, { 0,1,0,1 });

	//RigidSolid* solid = new RigidSolid(gPhysics, gScene, { 0,20,0 });


	/*
	char inst[4 * 3] = {
		0x00, 0x01, 0x25, 0xFF,

	};
	int (*func)(void) = (int (*)(void))((void *) & inst);
	int result = func();
	*/

	//solid->getPxRigidDynamic()->getMass();
	//solid->getPxRigidDynamic().getr


	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "prueba.obj");

	if (!success) {
		throw std::runtime_error("Failed to load OBJ: " + warn + err);
	}

	std::vector<physx::PxVec3> vertices;

	// Procesar malla (conviértela para PhysX)
	for (const auto& shape : shapes) {
		for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
			// Accede a los vértices de cada triángulo
			const tinyobj::index_t& v0 = shape.mesh.indices[i + 0];
			const tinyobj::index_t& v1 = shape.mesh.indices[i + 1];
			const tinyobj::index_t& v2 = shape.mesh.indices[i + 2];


			// Carga las posiciones de los vértices
			PxVec3 p0 = { attrib.vertices[3 * v0.vertex_index + 0],
						 attrib.vertices[3 * v0.vertex_index + 1],
						 attrib.vertices[3 * v0.vertex_index + 2] };
			PxVec3 p1 = { attrib.vertices[3 * v1.vertex_index + 0],
						 attrib.vertices[3 * v1.vertex_index + 1],
						 attrib.vertices[3 * v1.vertex_index + 2] };
			PxVec3 p2 = { attrib.vertices[3 * v2.vertex_index + 0],
						 attrib.vertices[3 * v2.vertex_index + 1],
						 attrib.vertices[3 * v2.vertex_index + 2] };

			// Añade los vértices a tu buffer o directamente a PhysX
			vertices.push_back(p0);
			vertices.push_back(p1);
			vertices.push_back(p2);
		}
	}




	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = vertices.size();
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = vertices.data(); // array de PxVec3

	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	// Cocina la malla convexa
	PxDefaultMemoryOutputStream writeBuffer;
	PxConvexMeshCookingResult::Enum result;
	bool status = gCooking->cookConvexMesh(convexDesc, writeBuffer, &result);

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxConvexMesh* convexMesh = gPhysics->createConvexMesh(readBuffer);

	// Añadir a la escena
	PxRigidDynamic* dynamicActor = gPhysics->createRigidDynamic(PxTransform({0,100,0}));
	//PxShape* shape = dynamicActor->createShape(PxConvexMeshGeometry(convexMesh), *material);
	PxShape* shape = CreateShape(PxConvexMeshGeometry(convexMesh));

	dynamicActor->attachShape(*shape);

	gScene->addActor(*dynamicActor);
	balaRenderItem = new RenderItem(shape, dynamicActor, { 0,1,0,1 });

}

ExampleSolidRigidScene::~ExampleSolidRigidScene()
{
	DeregisterRenderItem(floorRenderItem);

}

void ExampleSolidRigidScene::update(double t)
{
}

void ExampleSolidRigidScene::onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{

	//std::cout << "aaaa desde la escena" << std::endl;


}
