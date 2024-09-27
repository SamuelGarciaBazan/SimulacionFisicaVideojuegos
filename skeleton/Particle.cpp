#include "Particle.h"


using namespace physx;





Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acel,
	PxGeometryType::Enum type )
	:transform(pos), vel(vel), acel(acel)
{

	PxGeometry* g ;
	PxShape* shape;
	
	switch (type)
	{
	case physx::PxGeometryType::eSPHERE:
		g = new PxSphereGeometry(1);
		break;
	case physx::PxGeometryType::eCAPSULE:
		g = new PxCapsuleGeometry(1,2);
		break;
	case physx::PxGeometryType::eBOX:
		g = new PxBoxGeometry(1, 1, 1);
		break;

	default:
		g = new PxSphereGeometry(1);
		break;
	}
	
	shape = CreateShape(*g);
	
	PxVec4 color{ 1,1,1,1 };

	renderItem = new RenderItem(shape, &transform, color);

	delete g;

}




Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	transform.p += vel * t;
	
}
