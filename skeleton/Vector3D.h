#pragma once

#include <cmath>

template<typename T = float>
class Vector3D {

public:

	T x, y, z;

	Vector3D(T x,T y,T z)
		:x(x),y(y),z(z){}

	inline const Vector3D<T> normalized(){

		T mag = magnitude();
		
		return Vector3D<T>(x/mag,y/mag,z/mag);
	}

	inline void normalize() {

		T mag = magnitude();
		x = x / mag;
		y = y / mag;
		z = z / mag;
	}

	inline const T magnitudeSquare() {
		return (x*x)+(y*y)+(z*z);
	}

	inline const T magnitude() {
		return std::sqrt(magnitudeSquare())
	}

	//producto escalar
	inline T *operator(Vector3D<T>& const left, Vector3D<T>& const right) {
		return (left.x*right.x) + (left.y*right.y) + (left.z * right.z);
	}

	inline Vector3D<T>* operator(Vector3D<T>& const v, T e) {
		return Vector3D(v.x *e,v.y*e,v.z*e);
	}
};


