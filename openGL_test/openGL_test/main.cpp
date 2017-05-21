#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <math.h>
#include <iostream>

//#include "CollisionWall.h"
#include "Physics.h"
#include "Ball.h"

#define RADIUS 15 //Ball.cpp에도 있음.
#define RATIO 0.02 //당구대 크기에 따라 달라질 수 있음.
#define MAXLINE 2.236068*400
//#define WIDTH 400
//#define HEIGHT 400
//#define originX 50
//#define originY 50

using namespace std;

void lineTrace();

static bool	mouseLeftDown;
static float point[2][2];

static Vector3 line_end;
static Vector3 start_point;
static Vector3 end_point;
static Vector3 reflect_point[5];
static char count_reflect=-1;
static float e = 0.9;

//static float theta;


Vector3 yellowBallP(250, 120, 0);
Vector3 redBallP(200, 400, 0);
Vector3 whiteBallP(350, 350, 0);
//Vector3 yellowBallD(0, 0, 0);

Ball yellowBall(yellowBallP);
Ball redBall(redBallP);
Ball whiteBall(whiteBallP);

Wall wall;

//static float whiteBall[2][2];
//static float yellowBall[2][2];
//static float redBall[2][2];
//충격량

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//coordinate
	//glColor3f(1, 0, 0);
	//glBegin(GL_LINES);
	//	glVertex3f(-10,0,0);
	//	glVertex3f(10,0,0);
	//glEnd();
	//glColor3f(0, 1, 0);
	//glBegin(GL_LINES);
	//	glVertex3f(0, -10, 0);
	//	glVertex3f(0, 10, 0);
	//glEnd();
	//glColor3f(0, 0, 0);
	//glBegin(GL_LINES);
	//	glVertex3f(0.5, -0.1, 0);
	//	glVertex3f(0.5, 0.1, 0);
	//glEnd();

	//Balls
	glPointSize(RADIUS*2);
	glBegin(GL_POINTS);
		glColor3f(1, 0.9, 0.1);
		yellowBall.Draw();
		glColor3f(1, 0, 0);
		redBall.Draw();
		glColor3f(0.9, 0.9, 0.9);
		whiteBall.Draw();
	glEnd();

	//mouse line
	if (mouseLeftDown) {
		glEnable(GL_LINE_STIPPLE);

		glLineStipple(3, 0xcccc);
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex3f(start_point.x, start_point.y, start_point.z);// 2fv(point[0]);
			if (count_reflect >= 0) for (int i = 0; i <= count_reflect; i++) {
				glVertex3f(reflect_point[i].x, reflect_point[i].y, reflect_point[i].z);
				glEnd();
				//cout << "draw point" << count_reflect << endl;
				//cout << endl;
				glBegin(GL_LINES);
				glVertex3f(reflect_point[i].x, reflect_point[i].y, reflect_point[i].z);
			}
			glVertex3f(line_end.x, line_end.y, line_end.z);//v(point[1]);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
	}
	else;
		

	//boundary
	wall.Draw();


	glFlush();
}

void init(void)
{
	
	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;

}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Rendering Buffer 지울 색.
}

void ChangeSize(int w, int h) {
	if (h == 0)	h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);//reset coordinate system
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D(0, 500.0, 0, 500.0*(float)h / (float)w);
	else
		gluOrtho2D(0, 500.0*(float)w / (float)h, 0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void timer(int value)
{
	if (!mouseLeftDown) {
		yellowBall.move();
		redBall.move();
		whiteBall.move();
	}

	yellowBall.BoundaryCollision();
	redBall.BoundaryCollision();
	whiteBall.BoundaryCollision();

	yellowBall.collise(redBall, whiteBall);
//	redBall.collise(yellowBall, whiteBall);
//	whiteBall.collise(yellowBall, redBall);

	glutTimerFunc(3, timer, 1);
	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (!mouseLeftDown)
			{
				double viewport[4];
				glGetDoublev(GL_VIEWPORT, viewport);

				point[0][0] = x / (float)viewport[2] * 500;
				point[0][1] = (viewport[3] - y) / (float)viewport[3] * 900;
				point[1][0] = point[0][0];
				point[1][1] = point[0][1];
				start_point = Vector3(point[0][0], point[0][1], 0);
				cout << "glutdown," << endl;
				mouseLeftDown = true;
			}
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
			count_reflect = -1;
			cout << "glutup," << endl;
		}
	}
	glutPostRedisplay();//화면 갱신 필요할 때. display callback함수 재실행.
}
void mouseMotion(int x, int y)
{
	if (mouseLeftDown)
	{
		double viewport[4];
		glGetDoublev(GL_VIEWPORT, viewport);
		//cout << "viewport : (" << viewport[0] << "," << viewport[1] << "," << viewport[2] << "," << viewport[3] << ")" << endl;

		point[1][0] = x / (float)viewport[2] * 500;
		point[1][1] = (viewport[3] - y) / (float)viewport[3] * 900;
		//cout << "point(" << point[1][0] << "," << point[1][1] << ")" << endl;

		
		end_point = Vector3(point[1][0], point[1][1], 0);
		line_end = start_point + (end_point - start_point).normalize() * MAXLINE;
		
		count_reflect = -1;
		lineTrace();


		////////노란공 백터////////
		if (start_point.distance(yellowBall.transform->position) <=RADIUS) {
			yellowBall.transform->velocity.x = (point[1][0] - yellowBall.transform->position.x);
			yellowBall.transform->velocity.y = (point[1][1] - yellowBall.transform->position.y);
			//cout << "velocity(" << yellowBall.transform->velocity.x << "," << yellowBall.transform->velocity.y << ")"<<"length : "<< yellowBall.transform->velocity.length();
			yellowBall.transform->velocity = yellowBall.transform->velocity * RATIO;//yellowBall.transform->velocity = yellowBall.transform->velocity.normalize() * 2;
			yellowBall.transform->SetAccel();
			start_point = yellowBall.transform->position;
			//cout << "unit : (" << "v" << yellowBall.transform->velocity.x << "," << yellowBall.transform->velocity.y << ")" << endl;
			//cout << "unit : (" << "a" << yellowBall.transform->GetAccel().x << "," << yellowBall.transform->GetAccel().y << ")" << endl;
		}
		else if (start_point.distance(redBall.transform->position) <= RADIUS) {
			redBall.transform->velocity.x = (point[1][0] - redBall.transform->position.x);
			redBall.transform->velocity.y = (point[1][1] - redBall.transform->position.y);
			redBall.transform->velocity = redBall.transform->velocity * RATIO;
			redBall.transform->SetAccel();
			start_point = redBall.transform->position;
		}
		else if (start_point.distance(whiteBall.transform->position) <= RADIUS) {
			whiteBall.transform->velocity.x = (point[1][0] - whiteBall.transform->position.x);
			whiteBall.transform->velocity.y = (point[1][1] - whiteBall.transform->position.y);
			whiteBall.transform->velocity = whiteBall.transform->velocity * RATIO;
			whiteBall.transform->SetAccel();
			start_point = whiteBall.transform->position;
		}

		
	}
	glutPostRedisplay();
}


float lineBallDistance(Vector3 start, Vector3 end, Vector3 ball) {
	float a = end.y - start.y;
	float b = start.x - end.x;
	float c = start.y*end.x - start.x*end.y;
	float distance;
	distance = fabs(a*ball.x + b*ball.y + c) / sqrt(a*a + b*b);
	return distance;
}
void lineBall(Vector3 &start, Vector3 &end, Vector3 &reflect_point, Vector3 ball) {
	float d1 = lineBallDistance(start, end, ball);
	float D = start.distance(ball);
	reflect_point = start + ((end - start).normalize()*(sqrt(D*D - d1*d1) - sqrt(4 * RADIUS*RADIUS - d1*d1)));
	Vector3 thisdir = end - reflect_point;
	Vector3 tempdir = ball - reflect_point;
	tempdir.normalize();
	line_end = reflect_point + (thisdir - tempdir*((thisdir.x*tempdir.x + thisdir.y*tempdir.y))*e)*MAXLINE;
}
void lineWall(Vector3 start) {
	/*벽좌표*/
	float RIGHTWALL = 450;
	float LEFTWALL = 50;
	float TOPWALL = 850;
	float BOTTOMWALL = 50;

	if (line_end.x > RIGHTWALL - RADIUS) {
		reflect_point[count_reflect] = start + ((line_end - start) / (line_end.x - start.x)) *(RIGHTWALL - RADIUS - start.x);
		line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x*(-1)), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
		cout << "right" << endl;
		if (reflect_point[count_reflect].y < BOTTOMWALL + RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (start.y - reflect_point[count_reflect].y))*(start.y - BOTTOMWALL - RADIUS);
			line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x, (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "right + bottom" << endl;
		}
		else if (reflect_point[count_reflect].y > TOPWALL - RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (reflect_point[count_reflect].y - start.y)) *(TOPWALL - RADIUS - start.y);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "right+top" << endl;
		}
		//cout << "reflect_point : (" << reflect_point[count_reflect].x << "," << reflect_point[count_reflect].y << ")" << endl;
		//cout << "first vector : ("<< (((end - start) / (end.x - start.x))).x << "," << (((end - start) / (end.x - start.x))).y << ")" << endl;
		//cout << "second vector :(" << ((reflect_point[count_reflect] - start).x*(-1)) << "," << (reflect_point[count_reflect] - start).y <<  ")" << endl;
		//cout << "line_end : (" << line_end.x << "," << line_end.y << "," << line_end.z << ")" << endl;
		//cout << endl;
	}
	else if (line_end.x < LEFTWALL+ RADIUS) {
		reflect_point[count_reflect] = start + ((line_end - start) / (start.x - line_end.x))*(start.x - LEFTWALL- RADIUS);
		line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x*(-1), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
		cout << "left" << endl;
		if (reflect_point[count_reflect].y < BOTTOMWALL + RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (start.y - reflect_point[count_reflect].y))*(start.y - BOTTOMWALL - RADIUS);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "left_bottom" << endl;
		}
		else if (reflect_point[count_reflect].y > TOPWALL - RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (reflect_point[count_reflect].y - start.y)) *(TOPWALL- RADIUS - start.y);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "left+top" << endl;
		}
	}
	else if (line_end.y > TOPWALL - RADIUS) {
		reflect_point[count_reflect] = start + ((line_end - start) / (line_end.y - start.y)) *(TOPWALL - RADIUS - start.y);
		line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
		cout << "top" << endl;
		if (reflect_point[count_reflect].x > RIGHTWALL - RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (reflect_point[count_reflect].x - start.x)) *(RIGHTWALL - RADIUS - start.x);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x*(-1)), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "top+right" << endl;
		}
		else if (reflect_point[count_reflect].x < LEFTWALL + RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (start.x - reflect_point[count_reflect].x))*(start.x - LEFTWALL - RADIUS);
			line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x*(-1), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "top + left" << endl;
		}
	}
	else if (line_end.y < BOTTOMWALL + RADIUS) {
		reflect_point[count_reflect] = start + ((line_end - start) / (start.y - line_end.y))*(start.y - BOTTOMWALL - RADIUS);
		line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x, (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
		cout << "bottom" << endl;
		if (reflect_point[count_reflect].x > RIGHTWALL - RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (reflect_point[count_reflect].x - start.x)) *(RIGHTWALL - RADIUS - start.x);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x*(-1)), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "bottom+right" << endl;
		}
		else if (reflect_point[count_reflect].x < LEFTWALL + RADIUS) {
			reflect_point[count_reflect] = start + ((reflect_point[count_reflect] - start) / (start.x - reflect_point[count_reflect].x))*(start.x - LEFTWALL - RADIUS);
			line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x*(-1), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "bottom+left" << endl;
		}
	}
}
bool planEq(Vector3 start, Vector3 ball,Vector3 X) {//공과 시작점의 벡터를 법선벡터로하는 평면의 방정식.X가 평면위인지 아래인지 확인.
	Vector3 n = ball - start;
	Vector3 ap = X - start;
	if (((n.x*ap.x) + (n.y*ap.y)) > 0) return 1;//0보다 크면1
	else return 0;
}
void lineTrace()
{
	Vector3 start = start_point;
	//cout << "line_end : (" << line_end.x << "," << line_end.y << "," << line_end.z << ")" << endl;
	/*노란공을 움직인다면*/
	if (start_point.distance(yellowBall.transform->position) <= RADIUS) {
		for (int i = 0; i < 5; i++) {
		count_reflect++;

		float d1 = lineBallDistance(start, line_end, redBall.transform->position);
		float d2 = lineBallDistance(start, line_end, whiteBall.transform->position);
		bool b1 = planEq(start, redBall.transform->position, redBall.transform->position);
		bool b2 = planEq(start, whiteBall.transform->position, whiteBall.transform->position);
		cout << "b2 : "<<b2<< endl;
		if ((d1 < (2.0 * RADIUS)) && (planEq(start, redBall.transform->position, end_point) == b1) 
			&&!(fabs((2.0 * RADIUS) - (start.distance(redBall.transform->position))) <= FLT_EPSILON)) {//빨간공과 부딪친다면

			cout << "redball, 3거리 :"<< start.distance(redBall.transform->position) << endl;
			lineBall(start, line_end, reflect_point[count_reflect], redBall.transform->position);
		}
		else if ((d2 < (2 * RADIUS)) && (planEq(start, whiteBall.transform->position, end_point) == b2)) {//흰공과 부딪친다면
			cout << "wball, "<< planEq(start, whiteBall.transform->position, end_point) << endl;
			lineBall(start, line_end, reflect_point[count_reflect], whiteBall.transform->position);
		}
		/*Wall collision 8가지 경우*/
		else lineWall(start);
		//여기서 reflect_point[count_reflect] 를 중심으로하는 당구공(반투명?) 그려주면 더 좋을 듯.
		start = reflect_point[count_reflect];
		}
	}
	/*흰공을 움직인다면*/
	else if (start_point.distance(whiteBall.transform->position) <= RADIUS) {
		for (int i = 0; i < 5; i++) {
			count_reflect++;

			float d1 = lineBallDistance(start, line_end, redBall.transform->position);
			float d2 = lineBallDistance(start, line_end, yellowBall.transform->position);
			bool b1 = planEq(start, redBall.transform->position, redBall.transform->position);
			bool b2 = planEq(start, yellowBall.transform->position, yellowBall.transform->position);
			if ((d1 < (2 * RADIUS)) && (planEq(start, redBall.transform->position, end_point) == b1)) {//빨간공과 부딪친다면(반대쪽 처리해야함.)
				cout << "redball" << endl;
				lineBall(start, line_end, reflect_point[count_reflect], redBall.transform->position);
			}
			else if ((d2 < (2 * RADIUS)) && (planEq(start, yellowBall.transform->position, end_point) == b2)) {//노란공과 부딪친다면
				cout << "yball" << endl;
				lineBall(start, line_end, reflect_point[count_reflect], yellowBall.transform->position);
			}
			/*Wall collision 8가지 경우*/
			else lineWall(start);
			//여기서 reflect_point[count_reflect] 를 중심으로하는 당구공(반투명?) 그려주면 더 좋을 듯.
			start = reflect_point[count_reflect];
		}
	}
	cout << endl;
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q'|'Q':
		exit(0); break;
	case VK_ESCAPE:
		exit(0); break;
	default:
		break;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(316,579);
	glutCreateWindow("Test_hyeon");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutTimerFunc(3, timer, 1);

	init();
	SetupRC();

	glutMainLoop();
}