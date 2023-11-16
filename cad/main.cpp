#include<iostream>
#include<glut.h>
#include"halfedge_structure.h"
#include"eularoperation.h"
#define PI 3.1415926536
using namespace std;
double xrot = 0.0;
double yrot = 0.0;
double xrotr = 0.0;
double yrotr = 0.0;
double points_first[] = { 1.0, 1.0, 0.0 };
double points_second[] = { -1.0, 1.0, 0.0 }; 
double points_third[] = { -1.0, -1.0, 0.0 }; 
double points_forth[] = { 1.0, -1.0, 0.0 }; 
double points_1_first[4][3] = { 0.75, 0.75, 0.0,
								-0.25, 0.75, 0.0,
								-0.25, -0.25, 0.0,
								0.75, -0.25, 0.0};
double points_1_second[4][3] = { 0.25, 0.75, 0.0,
								-0.75, 0.75, 0.0,
								-0.75, -0.25, 0.0,
								0.25, -0.25, 0.0};
double points_1_third[4][3] = { 0.25, 0.25, 0.0,
								-0.75, 0.25, 0.0,
								-0.75, -0.75, 0.0,
								0.25, -0.75, 0.0};
double points_1_forth[4][3] = { 0.75, 0.25, 0.0,
								-0.25, 0.25, 0.0,
								-0.25, -0.75, 0.0,
								0.75, -0.75, 0.0};
double dir[] = { 0.0,0.0, 1.0 };
double d = 2.0;
bool displaymode = false;
int xp, yp;
bool bdown = false;
int num_ring = 0;


void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	Solid *s = mvfs(points_first);
	Loop* lp_out = s->facelist->loopout;
	
	mev(s->firstv, points_second, lp_out);
	mev(s->newv, points_third, lp_out);
	mev(s->newv, points_forth, lp_out);
	
	Loop* lp = mef(s->newv, s->firstv, lp_out);

	AddToSolidList(s);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-2.0, 0.0, -10.0);
	glRotatef(xrot + xrotr, 1.0, 0.0, 0.0);
	glRotatef(yrot + yrotr, 0.0, 1.0, 0.0);
	Solid* tmp;
	tmp = solidlist;
	while (tmp) {
		if (!displaymode)
			tmp->wirepaint();
		else
			tmp->solidpaint();
		tmp = tmp->next;
		if (tmp == solidlist)
			break;
		glTranslatef(3.0, 0, 0);
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(double)w/(double)h,0.1,100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			bdown = true;
			xp = x;
			yp = y;
		}
		else
		{
			bdown = false;
			xrot += xrotr;
			yrot += yrotr;
		}
	}
}
void move(int x, int y)
{
	int deltx, delty;
	if (bdown)
	{
		deltx = x - xp;
		delty = y - yp;
		yrotr = ((double)deltx / 10.0 * 180.0 / PI * 0.04);
		xrotr = ((double)delty / 10.0 * 180.0 / PI * 0.04);
	}
	glutPostRedisplay();
}

void create_ring() {
	Solid* s;
	s = solidlist;
	Face *f= s->facelist;
	Loop* lp_out = f->loopout;
	Loop* lp = lp_out;
	mev(s->firstv, points_1_first[num_ring], lp);
	HalfEdge* he = mev(s->newv, points_1_second[num_ring], lp);
	Vertex* v = he->vertex;
	mev(s->newv, points_1_third[num_ring], lp);
	mev(s->newv, points_1_forth[num_ring], lp);
	Loop* lp_in = mef(s->newv, v, lp);
	kemr(s->firstv, v, lp);
	kfmrh(lp_out->face, lp_in->face);
	num_ring++;
}

void processMenuEvent(int option)
{
	switch (option)
	{
	case 1:
		displaymode = false;
		glutPostRedisplay();
		break;
	case 2:
		displaymode = true;
		glutPostRedisplay();
		break;
	case 3:
		sweep(solidlist->facelist->next, dir, d);
		glutPostRedisplay();
		break;
	case 4:
		create_ring();
		break;
	default:break;
	}
}
void glCreateMenu(void)
{
	int menu;
	menu = glutCreateMenu(processMenuEvent);
	glutAddMenuEntry("wire cube", 1);
	glutAddMenuEntry("solid cube", 2);
	glutAddMenuEntry("sweep model", 3);
	glutAddMenuEntry("create ring", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(200, 150);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Euler");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glCreateMenu();
	glutMainLoop();
	return 0;
}
