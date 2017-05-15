#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#include "Ball.h"
#include "Physics.h"

#define RADIUS 5

float GameObject::HEIGHT = 400;
float GameObject::WIDTH = 400;
float GameObject::originX = 50;
float GameObject::originY = 50;


Ball::Ball()
{
}

Ball::Ball(Vector3 pos):GameObject(pos)
{
}


Ball::~Ball()
{
}

void Ball::move()
{
	this->transform->position.x += this->transform->velocity.x;
	this->transform->position.y += this->transform->velocity.y;
	//printf("%f, %f, %f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
	this->transform->velocity = this->transform->velocity - this->transform->GetAccel(); //this->transform->velocity -= this->transform->velocity * 0.001;
	//printf("GetAccel : %f,%f,%f\n", this->transform->InitAccel.x, this->transform->InitAccel.y, this->transform->InitAccel.z);
	//printf("%f, %f, %f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);


	if (fabs(this->transform->velocity.x) < 0.000001) this->transform->velocity.x = 0;
	if (fabs(this->transform->velocity.y) < 0.000001) this->transform->velocity.y = 0;
	this->transform->SetAccel();
	/*운동량_질량 = 50;*/
	this->transform->p = Vector3(50 * this->transform->velocity.x, 50 * this->transform->velocity.y, 50 * this->transform->velocity.z);
}

void Ball::BoundaryCollision()
{
	if (this->transform->position.x <= originX) {
		this->transform->position.x = originX;
		this->transform->velocity.x = -this->transform->velocity.x;
	}
	else if (this->transform->position.x >= originX + WIDTH) {
		this->transform->position.x = originX + WIDTH;
		this->transform->velocity.x = -this->transform->velocity.x;
	}
	if (this->transform->position.y <= originY) {
		this->transform->position.y = originY;
		this->transform->velocity.y = -this->transform->velocity.y;
	}
	else if (this->transform->position.y >= originY + HEIGHT) {
		this->transform->position.y = originY + HEIGHT;
		this->transform->velocity.y = -this->transform->velocity.y;
	}
}

void Ball::collise(Ball &A,Ball &B)//A,B,C 완전탄성운동
{
	if (A.transform->position.distance(this->transform->position) <= 2*RADIUS) {
		Vector3 temp;

		temp = A.transform->velocity;
		A.transform->velocity = (this->transform->velocity*0.5) - (A.transform->velocity*0.5);// *0.9;
		//printf("공 속도 : %f,%f,%f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
		this->transform->velocity = (temp*0.5)-(this->transform->velocity*0.5);
		//printf("바뀐 공 속도 : %f,%f,%f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
		this->transform->SetAccel();
		A.transform->SetAccel();
	}
	else if (B.transform->position.distance(this->transform->position) <= 2 * RADIUS) {
		Vector3 temp;
		
		/*temp = A.transform->velocity;
		B.transform->velocity = this->transform->velocity;
		this->transform->velocity = temp;*/
		temp = B.transform->velocity;
		B.transform->velocity = (this->transform->velocity*0.5) - (B.transform->velocity*0.5);// *0.9;
																							  //printf("공 속도 : %f,%f,%f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
		this->transform->velocity = (temp*0.5) - (this->transform->velocity*0.5);
		this->transform->SetAccel();
		B.transform->SetAccel();
	}
	else if (A.transform->position.distance(B.transform->position) <= 2 * RADIUS) {
		Vector3 temp;
		//temp = this->transform->velocity * (RADIUS / this->transform->velocity.length());
		//this->transform->position = B.transform->position - temp * 2.0;

		/*temp = B.transform->velocity;
		B.transform->velocity = A.transform->velocity;
		A.transform->velocity = temp;*/
		temp = A.transform->velocity;
		A.transform->velocity = (B.transform->velocity*0.5) - (A.transform->velocity*0.5);// *0.9;
																							  //printf("공 속도 : %f,%f,%f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
		B.transform->velocity = (temp*0.5) - (B.transform->velocity*0.5);
		A.transform->SetAccel();
		B.transform->SetAccel();
	}
}

void Ball::Draw()
{
	glVertex3f(this->transform->position.x, this->transform->position.y, this->transform->position.z);
}

GameObject::GameObject()
{
	transform = new Transform();
	//Collider = new GameObject();
	//vertexes[] =
}

GameObject::GameObject(Vector3 pos)
{
	transform = new Transform(pos);
	//Collider = new GameObject();
	//vertexes[] =
}

GameObject::~GameObject()
{
}

void GameObject::move()
{
}

void GameObject::collise()
{
}

Wall::Wall()
{
}

Wall::~Wall()
{
}

void Wall::Draw()
{
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 0);
		glVertex2f(GameObject::originX, GameObject::originY);
		glVertex2f(GameObject::originX + GameObject::WIDTH, GameObject::originY);
		glVertex2f(GameObject::originX + GameObject::WIDTH, GameObject::originY + GameObject::HEIGHT);
		glVertex2f(GameObject::originX, GameObject::originY + GameObject::HEIGHT);
	glEnd();
}
