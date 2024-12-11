#pragma once

#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"



class RigidSolid
{
public:

	RigidSolid(
		physx::PxPhysics* gPhysics,			//physics
		physx::PxScene* gScene,			//scene
		physx::PxVec3 pos = physx::PxVec3(0,0,0),	//posicion inicial
		physx::PxVec3 scale = physx::PxVec3(1,1,1),//escala
		double density = 0.15,				//densidad
		physx::PxGeometryType::Enum type = physx::PxGeometryType::Enum::eSPHERE //tipo de geometria
	); 

	~RigidSolid();

	physx::PxRigidDynamic* getPxRigidDynamic() noexcept {
		return rigidDynamic;
	}


private:

	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;

	RenderItem* renderItem = nullptr;

	physx::PxRigidDynamic* rigidDynamic = nullptr;
	
};

