#pragma once

#include <cmath>

template<typename T = float>
class Vector3D {

public:

	T x, y, z;

	Vector3D(){}

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
	inline T operator*( Vector3D<T>& const other) const {
		return (x* other.x) + (y* other.y) + (z * other.z);
	}


	//producto por un escalar
	inline Vector3D<T> operator*( T const scalar) const {
		return Vector3D<T>(x * scalar,y* scalar,z* scalar);
	}
	//escalar por un producto
	friend inline Vector3D<T> operator*(T scalar, const Vector3D<T>& vec) {
		return Vector3D<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	
	//suma
	inline Vector3D<T> operator+(Vector3D<T>& const other) const {
		return Vector3D<T>(x + other.x, y + other.y, z + other.z);
	}

	//resta
	inline Vector3D<T> operator-(Vector3D<T>& const other) const {
		return Vector3D<T>(x - other.x, y - other.y, z - other.z);
	}

	//igualdad
	inline bool operator==(const Vector3D<T>& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	inline T dot(Vector3D<T>& const  other) const {

		return (*this) * other;
	}

	inline Vector3D<T> cross(Vector3D<T>& const  b) const {

		return Vector3D( (y * b.z - b.y * z),
						 (z * b.x - b.z * x),
						 (x * b.y - b.x * y) );
	}

};


