#pragma once

#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <list>


class RigidSolid
{
public:

	RigidSolid(
		std::list<RigidSolid*>& allRigidSolids, //lista de solidos rigidos
		physx::PxPhysics* gPhysics,						//physics
		physx::PxScene* gScene,							//scene
		physx::PxVec3 pos = physx::PxVec3(0,0,0),		//posicion inicial
		physx::PxVec3 scale = physx::PxVec3(1,1,1),		//escala
		physx::PxVec4 color = physx::PxVec4(1,1,1,1),	//color
		double density = 0.15,							//densidad
		physx::PxGeometryType::Enum type = physx::PxGeometryType::Enum::eSPHERE //tipo de geometria
	); 

	~RigidSolid();

	physx::PxRigidDynamic* getPxRigidDynamic() noexcept {
		return rigidDynamic;
	}

	void setForceIndependent(bool b) {
		forceIndependent = b;
	}

	bool getForceIndependent() const noexcept {
		return forceIndependent;
	}

private:

	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;

	RenderItem* renderItem = nullptr;

	physx::PxRigidDynamic* rigidDynamic = nullptr;
	
	std::list<RigidSolid*>& allRigidSolids;
	std::list<RigidSolid*>::iterator myIt;

	bool forceIndependent = false;
};

