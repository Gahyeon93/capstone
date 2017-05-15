#include "CollisionWall.h"
#include "Physics.h"
#include <math.h>

/*int CollisionWall::SetWall(float Width, float height, float originx, float originy)
{
	WIDTH = Width;
	HEIGHT = height;
	originX = originx;
	originY = originy;
	return 0;
}
*/
void CollisionWall::BoundaryCollision(Vector3 &position, Vector3 &direction) //timer
{
	if (position.x <= originX) {
		position.x = originX;
		direction.x = -direction.x;
	}
	else if (position.x >= originX + WIDTH) {
		position.x = originX + WIDTH;
		direction.x = -direction.x;
	}
	if (position.y <= originY) {
		position.y = originY;
		direction.y = -direction.y;
	}
	else if (position.y >= originY + HEIGHT) {
		position.y = originY + HEIGHT;
		direction.y = -direction.y;
	}
}

CollisionWall::CollisionWall()
{
}


CollisionWall::~CollisionWall()
{
}
