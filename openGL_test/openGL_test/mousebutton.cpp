/*
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

static bool	mouseLeftDown;
static float point[2][2];

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_STIPPLE);

	if (mouseLeftDown)
		glLineStipple(3, 0xcccc);
	else
		glDisable(GL_LINE_STIPPLE);

	glColor3f(0, 0, 1);
	glLineWidth(2);

	glBegin(GL_LINES);
		glVertex2fv(point[0]);
		glVertex2fv(point[1]);
	glEnd();
	
	//glLineWidth(5);
	//glBegin(GL_LINE_LOOP);
	//	glColor3f(1, 0, 0);
	//	glVertex2f(-50, -50);
	//	glColor3f(0, 1, 0);
	//	glVertex2f(0, -50);
	//	glColor3f(0, 0, 1);
	//	glVertex2f(0, 0);
	//	glColor3f(0, 0, 0);
	//	glVertex2f(-50, 0);
	//glEnd();
	
	glFlush();
}

void init(void)
{
	mouseLeftDown = false;

	point[0][0] = 0;
	point[0][1] = 0;
	point[1][0] = 0;
	point[1][1] = 0;
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(int w, int h)
{
	if (h == 0)	h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);//reset coordinate system
	glLoadIdentity();

	if (w <= h)
		gluOrtho2D(0, 500.0, 0, 500.0*(float)h / (float)w);
	else
		gluOrtho2D(0, 500.0*(float)w / (float)h, 0, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
				point[0][1] = (viewport[3] - y) / (float)viewport[3] * 500;
				point[1][0] = point[0][0];
				point[1][1] = point[0][1];

				mouseLeftDown = true;
			}
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN) {
		}
		else if (state == GLUT_UP) {
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

		point[1][0] = x / (float)viewport[2] * 500;
		point[1][1] = (viewport[3] - y) / (float)viewport[3] * 500;
	}
	glutPostRedisplay();
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600,600);
	glutCreateWindow("Test_hyeon");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);

	init();
	SetupRC();

	glutMainLoop();
}*/