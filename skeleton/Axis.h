#pragma once

#include <PxPhysicsAPI.h>

#include <vector>
#include "RenderUtils.hpp"

#include "Vector3D.h"



class Axis {

private:

	std::vector <physx::PxShape*> shapes;
	std::vector <physx::PxTransform*> transforms;
	std::vector <physx::PxVec4> colors;
	std::vector <RenderItem*> renderItems;

	int nSpheres = 4;

public:

	Axis(int radiusAxis = 10, int radiusPoints = 1) {


		shapes.reserve(nSpheres);
		transforms.reserve(nSpheres);
		renderItems.reserve(nSpheres);

		for (int i = 0; i < nSpheres; i++) {
			shapes.push_back(CreateShape(physx::PxSphereGeometry(radiusPoints)));
		}

		Vector3D zero(0, 0, 0);
		Vector3D x(radiusAxis, 0, 0);
		Vector3D y(0, radiusAxis, 0);
		Vector3D z(0, 0, radiusAxis);


		transforms.push_back(new physx::PxTransform(physx::PxVec3(zero.x, zero.y, zero.z)));
		transforms.push_back(new physx::PxTransform(physx::PxVec3(x.x, x.y, x.z)));
		transforms.push_back(new physx::PxTransform(physx::PxVec3(y.x, y.y, y.z)));
		transforms.push_back(new physx::PxTransform(physx::PxVec3(z.x, z.y, z.z)));

		colors.push_back(physx::PxVec4(1, 1, 1, 1));
		colors.push_back(physx::PxVec4(1, 0, 0, 1));
		colors.push_back(physx::PxVec4(0, 1, 0, 1));
		colors.push_back(physx::PxVec4(0, 0, 1, 1));

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