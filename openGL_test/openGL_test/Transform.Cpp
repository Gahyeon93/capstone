#include"Physics.h"
#include"Matrix.h"
#include <stdio.h>
class Matrix;

Transform::Transform()
{
	forward = Vector3(0, 0, 1);
	upVector = Vector3(0, 1, 0);
	leftVector = Vector3(1, 0, 0);
	position = Vector3(0, 0, 0);
	velocity = Vector3(0, 0, 0);
	p = Vector3(0, 0, 0);
	InitAccel = Vector3(0, 0, 0);
}

Transform::Transform(Vector3 pos) : position(pos)
{
	forward = Vector3(0, 0, 1);	
	upVector = Vector3(0, 1, 0);
	leftVector = Vector3(1, 0, 0);
	velocity = Vector3(0, 0, 0);
	p = Vector3(0, 0, 0);
	InitAccel = Vector3(0, 0, 0);
}

void Transform::SetAccel()
{
	Vector3 temp = this->velocity;
	//printf("velocity : %f,%f,%f\n", this->velocity.x, this->velocity.y, this->velocity.z);
	this->InitAccel = Vector3(0, 0, 0);
	//printf("InitAccel : %f,%f,%f\n", this->InitAccel.x, this->InitAccel.y, this->InitAccel.z);
	this->InitAccel = temp.normalize()*0.005;
	//if (x) this->InitAccel.x = this->InitAccel.x + (this->velocity.x * 0.005);
	//if (y) this->InitAccel.y = this->InitAccel.y + (this->velocity.y * 0.005);
	//if (z) this->InitAccel.z = this->InitAccel.z + (this->velocity.z * 0.005);
	//printf("SetAccel : %f,%f,%f\n", this->InitAccel.x, this->InitAccel.y, this->InitAccel.z);
}

Vector3 Transform::GetAccel()
{
	return this->InitAccel;
}

const Vector3* const Transform::Getforward(void)
{
	return &forward;
}

Transform::~Transform()
{
}

const Vector3* const Transform::GetUp(void)
{
	return &upVector;
}
const Vector3* const Transform::GetLeft(void)
{
	return &leftVector;
}

void Transform::rotate(const float xAngle, const float yAngle, const float zAngle)
{
	Matrix xRotate(leftVector);

	xRotate = xRotate.Rotate(xAngle);
	upVector = xRotate*upVector;
	forward = xRotate*forward;


	Matrix yRotate(upVector);
	yRotate = yRotate.Rotate(yAngle);
	forward = yRotate*forward;
	leftVector = yRotate*leftVector;


	Matrix zRotate(forward);
	zRotate = zRotate.Rotate(zAngle);
	upVector = zRotate*upVector;
	leftVector = zRotate*leftVector;
}