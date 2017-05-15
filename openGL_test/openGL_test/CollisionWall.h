#pragma once
class Vector3;
class Transform;

#define WIDTH 400
#define HEIGHT 400
#define originX 50
#define originY 50

class CollisionWall
{
private:
	//Vector3;
public:
	/*int SetWall(float Width, float height, float originx, float originy);*/

	void BoundaryCollision(Vector3 &position, Vector3 &direction);
	CollisionWall();
	~CollisionWall();
};