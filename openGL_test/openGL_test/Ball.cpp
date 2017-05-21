#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#include "Ball.h"
#include "Physics.h"

#define RADIUS 15

float GameObject::HEIGHT = 800;
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
	//if (this->transform->velocity.x != 0 && this->transform->velocity.y != 0)
		this->transform->velocity = this->transform->velocity - this->transform->GetAccel();
//	if (this->transform->velocity.x)
//		this->transform->velocity.x = this->transform->velocity.x - this->transform->GetAccel().x; //this->transform->velocity -= this->transform->velocity * 0.001;
	//printf("GetAccel : %f,%f,%f\n", this->transform->InitAccel.x, this->transform->InitAccel.y, this->transform->InitAccel.z);
	//printf("%f, %f, %f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
//	if (this->transform->velocity.y)
//		this->transform->velocity.y = this->transform->velocity.y - this->transform->GetAccel().y;

	if (fabs(this->transform->velocity.x) < this->transform->GetAccel().x) {
		this->transform->velocity.x = 0;
	}
	if (fabs(this->transform->velocity.y) < this->transform->GetAccel().y) {
		this->transform->velocity.y = 0;
	}
	this->transform->SetAccel();
	//if (this->transform->velocity.x == 0) this->transform->SetAccel(0, 1, 1);
	//if (this->transform->velocity.y == 0)
//	this->transform->SetAccel(1, 0, 1);
	
	/*운동량_질량 = 50;*/
	this->transform->p = Vector3(50 * this->transform->velocity.x, 50 * this->transform->velocity.y, 50 * this->transform->velocity.z);
}

void Ball::BoundaryCollision()
{
	if (this->transform->position.x <= originX+RADIUS) {
		this->transform->position.x = originX + RADIUS;
		this->transform->velocity.x = -this->transform->velocity.x;
	}
	else if (this->transform->position.x >= originX + WIDTH - RADIUS) {
		this->transform->position.x = originX + WIDTH - RADIUS;
		this->transform->velocity.x = -this->transform->velocity.x;
	}
	if (this->transform->position.y <= originY + RADIUS) {
		this->transform->position.y = originY + RADIUS;
		this->transform->velocity.y = -this->transform->velocity.y;
	}
	else if (this->transform->position.y >= originY + HEIGHT - RADIUS) {
		this->transform->position.y = originY + HEIGHT - RADIUS;
		this->transform->velocity.y = -this->transform->velocity.y;
	}
}

void Ball::collise(Ball &A,Ball &B)//수정
{
	float e = 0.9;//탄성계수

	if (A.transform->position.distance(this->transform->position) <= 2*RADIUS) {
		Vector3 tempAv = A.transform->velocity;
		Vector3 tempTv = this->transform->velocity;

		/*this 충돌방향벡터*/
		Vector3 tempdir;
		tempdir = A.transform->position - this->transform->position;
		tempdir.normalize();

		this->transform->position = A.transform->position - (tempdir.normalize()*RADIUS * 2);//겹치지 않게 재배치

		float temptv;
		temptv = ((tempTv.x*tempdir.x) + (tempTv.y*tempdir.y)) / tempdir.length();//충돌방향으로의 this속도성분
		tempdir = tempdir*temptv;
		
		A.transform->velocity = A.transform->velocity + tempdir*e;
		this->transform->velocity = this->transform->velocity - tempdir*e;

		/*A 충돌방향벡터*/
		tempdir = this->transform->position - A.transform->position;
		tempdir.normalize();
		float tempav;
		tempav= ((tempAv.x*tempdir.x) + (tempAv.y*tempdir.y)) / tempdir.length();//충돌방향으로의 this속도성분
		tempdir = tempdir*tempav;
		A.transform->velocity = A.transform->velocity - tempdir*e;
		this->transform->velocity = this->transform->velocity + tempdir*e;
		
		//printf("공 속/도 : %f,%f,%f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);
		//printf("바뀐 공 속도 : %f,%f,%f\n", this->transform->velocity.x, this->transform->velocity.y, this->transform->velocity.z);

	}
	else if (B.transform->position.distance(this->transform->position) <= 2 * RADIUS) {
		Vector3 tempAv = B.transform->velocity;
		Vector3 tempTv = this->transform->velocity;

		/*this 충돌방향벡터*/
		Vector3 tempdir;
		tempdir = B.transform->position - this->transform->position;
		tempdir.normalize();
		this->transform->position = B.transform->position - (tempdir.normalize()*RADIUS * 2);//겹치지 않게 재배치

		float temptv;
		temptv = ((tempTv.x*tempdir.x) + (tempTv.y*tempdir.y)) / tempdir.length();//충돌방향으로의 this속도성분
		tempdir = tempdir*temptv;

		B.transform->velocity = B.transform->velocity + tempdir*e;
		this->transform->velocity = this->transform->velocity - tempdir*e;

		/*A 충돌방향벡터*/
		tempdir = this->transform->position - B.transform->position;
		tempdir.normalize();
		float tempav;
		tempav = ((tempAv.x*tempdir.x) + (tempAv.y*tempdir.y)) / tempdir.length();//충돌방향으로의 this속도성분
		tempdir = tempdir*tempav;
		B.transform->velocity = B.transform->velocity - tempdir*e;
		this->transform->velocity = this->transform->velocity + tempdir*e;
	}
	else if (A.transform->position.distance(B.transform->position) <= 2 * RADIUS) {
		Vector3 tempAv = B.transform->velocity;
		Vector3 tempTv = A.transform->velocity;

		/*this 충돌방향벡터*/
		Vector3 tempdir;
		tempdir = B.transform->position - A.transform->position;
		tempdir.normalize();
		A.transform->position = B.transform->position - (tempdir.normalize()*RADIUS * 2);//겹치지 않게 재배치

		float temptv;
		temptv = ((tempTv.x*tempdir.x) + (tempTv.y*tempdir.y)) / tempdir.length();//충돌방향으로의 this속도성분
		tempdir = tempdir*temptv;

		B.transform->velocity = B.transform->velocity + tempdir*e;
		A.transform->velocity = A.transform->velocity - tempdir*e;

		/*A 충돌방향벡터*/
		tempdir = A.transform->position - B.transform->position;
		tempdir.normalize();
		float tempav;
		tempav = ((tempAv.x*tempdir.x) + (tempAv.y*tempdir.y)) / tempdir.length();//충돌방향으로의 this속도성분
		tempdir = tempdir*tempav;
		B.transform->velocity = B.transform->velocity - tempdir*e;
		A.transform->velocity = A.transform->velocity + tempdir*e;
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



