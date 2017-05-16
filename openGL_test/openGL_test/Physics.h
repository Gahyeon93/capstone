#pragma once

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();

	Vector3 normalize();
	float distance(Vector3 input);
	float length();

	Vector3    operator *(const int nInput) const;
	Vector3    operator *(const float fInput) const;
	Vector3    operator *(const double dInput) const;
	Vector3	   operator -(const Vector3 Input) const;
	Vector3	   operator +(const Vector3 Input) const;

};



class Transform
{
private:
	Vector3 forward;
	Vector3 upVector;
	Vector3 leftVector;
	

public:
	Vector3 InitAccel;

	Vector3 velocity;
	Vector3 position;
	Vector3 p;//momentum

	Transform();
	Transform(Vector3 position);
	~Transform();

	//Functions
	void SetAccel();
	Vector3 GetAccel();

	const Vector3* const Getforward(void);
	const Vector3* const GetUp(void);
	const Vector3* const GetLeft(void);
	void rotate(const float xAngle, const float yAngle, const float zAngle);
};
