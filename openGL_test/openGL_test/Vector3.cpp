#include"Physics.h"
#include<math.h>

//∫§≈Õ¿‘¥œ¥Ÿ.

Vector3::Vector3()
{
	x = 0.0f; y = 0.0f; z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	(*this).x = x;
	(*this).y = y;
	(*this).z = z;
}

Vector3::~Vector3()
{
	
}

Vector3 Vector3::normalize()
{
	float l = sqrt(x*x + y*y + z*z);
	if (l == 0) return(*this);
	x /= l;
	y /= l ;
	z /= l;
	return (*this);
}

float Vector3::distance(Vector3 input)
{
	float dis;
	dis = sqrt((x - input.x)*(x - input.x) + (y - input.y)*(y - input.y));
	return dis;
}

float Vector3::length()
{
	float result;
	result = sqrt(x*x + y*y);
	return result;
}



Vector3    Vector3::operator*(const int nInput) const
{
	Vector3 result;
	result = *this;

	result.x = result.x*nInput;
	result.y = result.y*nInput;
	result.z = result.z*nInput;
	return result;
}
Vector3    Vector3::operator *(const float Input) const
{
	Vector3 result;
	result = *this;

	result.x = result.x*Input;
	result.y = result.y*Input;
	result.z = result.z*Input;
	return result;
}
Vector3    Vector3::operator *(const double Input) const
{
	Vector3 result;
	result = *this;

	result.x = result.x*Input;
	result.y = result.y*Input;
	result.z = result.z*Input;
	return result;
}

Vector3 Vector3::operator -(const Vector3 Input) const
{
	Vector3 result;
	result = *this;

	result.x -= Input.x;
	result.y -= Input.y;
	result.z -= Input.z;

	return result;
}

Vector3 Vector3::operator+(const Vector3 Input) const
{
	Vector3 result;
	result = *this;

	result.x += Input.x;
	result.y += Input.y;
	result.z += Input.z;

	return result;
	return Vector3();
}

Vector3 Vector3::operator/(const float Input) const
{
	Vector3 result;
	result = *this;

	result.x = result.x/Input;
	result.y = result.y/Input;
	result.z = result.z/Input;

	return result;
}
