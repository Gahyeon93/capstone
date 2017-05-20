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

//static float theta;


Vector3 yellowBallP(250, 120, 0);
Vector3 redBallP(250, 250, 0);
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
			glVertex2fv(point[0]);
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

		start_point = Vector3(point[0][0], point[0][1], 0);
		end_point = Vector3(point[1][0], point[1][1], 0);
		line_end = start_point + (end_point - start_point).normalize() * MAXLINE;
		
		count_reflect = -1;
		lineTrace();


		////////노란공 백터////////
		if (sqrt((point[0][0]-yellowBall.transform->position.x)*(point[0][0] - yellowBall.transform->position.x)+
				(point[0][1]- yellowBall.transform->position.y)*(point[0][1] - yellowBall.transform->position.y))<=RADIUS) {
			yellowBall.transform->velocity.x = (point[1][0] - yellowBall.transform->position.x);
			yellowBall.transform->velocity.y = (point[1][1] - yellowBall.transform->position.y);
			//cout << "velocity(" << yellowBall.transform->velocity.x << "," << yellowBall.transform->velocity.y << ")"<<"length : "<< yellowBall.transform->velocity.length();
			yellowBall.transform->velocity = yellowBall.transform->velocity * RATIO;//yellowBall.transform->velocity = yellowBall.transform->velocity.normalize() * 2;
			yellowBall.transform->SetAccel();
			//cout << "unit : (" << "v" << yellowBall.transform->velocity.x << "," << yellowBall.transform->velocity.y << ")" << endl;
			//cout << "unit : (" << "a" << yellowBall.transform->GetAccel().x << "," << yellowBall.transform->GetAccel().y << ")" << endl;
		}
		else if (sqrt((point[0][0] - redBall.transform->position.x)*(point[0][0] - redBall.transform->position.x) +
				(point[0][1] - redBall.transform->position.y)*(point[0][1] - redBall.transform->position.y)) <= RADIUS) { 
			redBall.transform->velocity.x = (point[1][0] - redBall.transform->position.x);
			redBall.transform->velocity.y = (point[1][1] - redBall.transform->position.y);
			redBall.transform->velocity = redBall.transform->velocity * RATIO;
			redBall.transform->SetAccel();
		}
		else if (sqrt((point[0][0] - whiteBall.transform->position.x)*(point[0][0] - whiteBall.transform->position.x) +
				(point[0][1] - whiteBall.transform->position.y)*(point[0][1] - whiteBall.transform->position.y)) <= RADIUS) {
			whiteBall.transform->velocity.x = (point[1][0] - whiteBall.transform->position.x);
			whiteBall.transform->velocity.y = (point[1][1] - whiteBall.transform->position.y);
			whiteBall.transform->velocity = whiteBall.transform->velocity * RATIO;
			whiteBall.transform->SetAccel();
		}

		
	}
	glutPostRedisplay();
}
float lineBallDis(Vector3 start, Vector3 end, Vector3 O) {
	float a = end.y - start.y;
	float b = start.x - end.x;
	float c = start.y*end.x - start.x*end.y;
	float distance;
	distance = fabs(a*O.x + b*O.y + c)/sqrt(a*a + b*b);
	return distance;
}
void lineTrace()
{
	float RIGHTWALL = 450;
	float LEFTWALL = 50;
	float TOPWALL = 850;
	float BOTTOMWALL = 50;

	Vector3 start = start_point;
	Vector3 end = line_end;
	cout << "line_end : (" << line_end.x << "," << line_end.y << "," << line_end.z << ")" << endl;
	/*노란공을 움직인다면*/
	if (sqrt((point[0][0] - yellowBall.transform->position.x)*(point[0][0] - yellowBall.transform->position.x) +
		(point[0][1] - yellowBall.transform->position.y)*(point[0][1] - yellowBall.transform->position.y)) <= RADIUS) {
	//	//for (int i = 0; i < 5; i++) {
		count_reflect++;

		float d1 = lineBallDis(start, end, redBall.transform->position);
		float d2 = lineBallDis(start, end, whiteBall.transform->position);
		if (d1 < (2 * RADIUS)) {//빨간공과 부딪친다면(반대쪽 처리해야함.)
			cout << "redball" << endl;
			float D = start.distance(redBall.transform->position);
			reflect_point[count_reflect] = start + ((end - start).normalize()*(sqrt(D*D - d1*d1) - sqrt(4 * RADIUS*RADIUS - d1*d1)));
	//		Vector3 thisdir = end - reflect_point[count_reflect];
	//		Vector3 tempdir = redBall.transform->position - redBall.transform->position;
			line_end = Vector3(0, 0, 0);
	//		line_end = reflect_point[count_reflect] + (end - reflect_point[count_reflect]) - tempdir*((thisdir.x*tempdir.x + thisdir.y*tempdir.y) / tempdir.length());
		}
		else if (d2 < (2 * RADIUS)) {//흰공과 부딪친다면
			cout << "wball" << endl;
									 //		float D = start.distance(whiteBall.transform->position);
	//		reflect_point[count_reflect] = start + ((end - start).normalize()*(sqrt(D*D - d2*d2) - sqrt(4 * RADIUS*RADIUS - d2*d2)));
	//		Vector3 thisdir = end - reflect_point[count_reflect];
	//		Vector3 tempdir = whiteBall.transform->position - whiteBall.transform->position;
	//		line_end = reflect_point[count_reflect] + (end - reflect_point[count_reflect]) - tempdir*((thisdir.x*tempdir.x + thisdir.y*tempdir.y) / tempdir.length());
		}
		/*Wall collision 8가지 경우*/
		else if (line_end.x > RIGHTWALL - RADIUS) {
			reflect_point[count_reflect] = start + ((end - start) / (end.x - start.x)) *(RIGHTWALL - RADIUS - start.x);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x*(-1)), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "right" << endl;
			if (reflect_point[count_reflect].y < BOTTOMWALL + RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (start.y - end.y))*(start.y - BOTTOMWALL - RADIUS);
				line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x, (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "right + bottom" << endl;
			}
			else if (reflect_point[count_reflect].y > TOPWALL - RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (end.y - start.y)) *(TOPWALL - RADIUS - start.y);
				line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "right+top" << endl;
			}
			//cout << "reflect_point : (" << reflect_point[count_reflect].x << "," << reflect_point[count_reflect].y << ")" << endl;
			//cout << "first vector : ("<< (((end - start) / (end.x - start.x))).x << "," << (((end - start) / (end.x - start.x))).y << ")" << endl;
			//cout << "second vector :(" << ((reflect_point[count_reflect] - start).x*(-1)) << "," << (reflect_point[count_reflect] - start).y <<  ")" << endl;
			//cout << "line_end : (" << line_end.x << "," << line_end.y << "," << line_end.z << ")" << endl;
			//cout << endl;
		}
		else if (line_end.x < LEFTWALL + RADIUS) {
			reflect_point[count_reflect] = start + ((end - start) / (start.x - end.x))*(start.x - LEFTWALL - RADIUS);
			line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x*(-1), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "left" << endl;
			if (reflect_point[count_reflect].y < BOTTOMWALL + RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (start.y - end.y))*(start.y - BOTTOMWALL - RADIUS);
				line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x, (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "left_bottom" << endl;
			}
			else if (reflect_point[count_reflect].y > TOPWALL - RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (end.y - start.y)) *(TOPWALL - RADIUS - start.y);
				line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "left+top" << endl;
			}
		}
		else if (line_end.y > TOPWALL - RADIUS) {
			reflect_point[count_reflect] = start + ((end - start) / (end.y - start.y)) *(TOPWALL - RADIUS - start.y);
			line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x), (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "top" << endl;
			if (reflect_point[count_reflect].x > RIGHTWALL - RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (end.x - start.x)) *(RIGHTWALL - RADIUS - start.x);
				line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x*(-1)), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "top+right" << endl;
			}
			else if (reflect_point[count_reflect].x < LEFTWALL + RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (start.x - end.x))*(start.x - LEFTWALL - RADIUS);
				line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x*(-1), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "top + left" << endl;
			}
		}
		else if (line_end.y < BOTTOMWALL + RADIUS) {
			reflect_point[count_reflect] = start + ((end - start) / (start.y - end.y))*(start.y - BOTTOMWALL - RADIUS);
			line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x, (reflect_point[count_reflect] - start).y*(-1), (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
			cout << "bottom" << endl;
			if (reflect_point[count_reflect].x > RIGHTWALL - RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (end.x - start.x)) *(RIGHTWALL - RADIUS - start.x);
				line_end = reflect_point[count_reflect] + Vector3(((reflect_point[count_reflect] - start).x*(-1)), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "bottom+right" << endl;
			}
			else if (reflect_point[count_reflect].x < LEFTWALL + RADIUS) {
				reflect_point[count_reflect] = start + ((end - start) / (start.x - end.x))*(start.x - LEFTWALL - RADIUS);
				line_end = reflect_point[count_reflect] + Vector3((reflect_point[count_reflect] - start).x*(-1), (reflect_point[count_reflect] - start).y, (reflect_point[count_reflect] - start).z).normalize()*MAXLINE;
				cout << "bottom+left" << endl;
			}
		}
		//이부분에서 reflect_point[count_reflect] 를 중심으로하는 당구공(반투명?) 그려주면 더 좋을 듯.

		start = reflect_point[count_reflect];
		end = start + (line_end - start).normalize();
		//}
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