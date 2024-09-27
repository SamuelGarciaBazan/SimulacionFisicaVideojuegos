#include "Particle.h"


using namespace physx;

Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acel)
	:transform(pos),vel(vel),acel(acel)
{
	
	PxShape* shape = CreateShape(PxSphereGeometry(1));
	PxVec4 color{ 1,1,1,1 };

	renderItem = new RenderItem(shape,&transform,color);

}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	transform.p += vel * t;
	
}
