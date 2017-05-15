#pragma once
class Vector3;
class Transform;
//class Collider;

class GameObject
{
protected:
	float *vertexes[4];

public:
	GameObject();
	GameObject(Vector3 pos);
	~GameObject();

	static float WIDTH;// 400
	static float HEIGHT;// 400
	static float originX;//50
	static float originY;//50

	Transform *transform;
	//GameObject *Collider;

	virtual void move();
	virtual void collise();
	virtual void Draw() = 0;
};

class Ball :public GameObject
{
private:

public:
	Ball();
	Ball(Vector3 pos);
	~Ball();
	void move();
	void BoundaryCollision();
	void collise(Ball &A, Ball &B);
	void Draw();
};

////////
class Wall : public GameObject
{
private:
public:
	Wall();
	~Wall();
	void Draw();
};

