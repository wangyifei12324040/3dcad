#ifndef _HALFEDGE_STRUCTUR_H
#define _HALFEDGE_STRUCTUR_H

class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;


class Vertex {
public:
	Vertex(double x, double y, double z) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
		id = 0;
		next = NULL;
		pre = NULL;
		halfedge = NULL;
	}
	int id;
	double coord[3];
	Vertex* pre;	
	Vertex* next;	
	HalfEdge* halfedge;
};

class HalfEdge {
public:
	HalfEdge() {
		edge = NULL;
		pre = NULL;
		next = NULL;
		brother = NULL;
		loop = NULL;
		vertex = NULL;
	}
	Edge* edge;
	HalfEdge* brother;
	HalfEdge* pre;	
	HalfEdge* next;		
	Loop* loop;
	Vertex* vertex;
};

class Edge {
public:
	Edge() {
		id = 0;
		pre = NULL;
		next = NULL;
		lhe = NULL;
		rhe = NULL;
	}
	int id;
	Edge* pre;	
	Edge* next;		
	HalfEdge* lhe;
	HalfEdge* rhe;
	Solid* solid;
};

class Loop {
public:
	Loop() {
		id = 0;
		pre = NULL;
		next = NULL;
		halfedgelist = NULL;
		face = NULL;
	}
	int id;
	Loop* pre;	
	Loop* next;		
	HalfEdge* halfedgelist;
	Face* face;
};

class Face {
public:
	Face() {
		id = 0;
		next = NULL;
		pre = NULL;
		solid = NULL;
		loopout = NULL;
		loopinlist = NULL;
	}
	int id;
	Face* next;	
	Face* pre;	
	Solid* solid;
	Loop* loopout;
	Loop* loopinlist;
};

class Solid {
public:
	Solid() {
		vnum = 0;
		fnum = 0;
		lnum = 0;
		ednum = 0;
		facelist = NULL;
		edgelist = NULL;
		firstv = NULL;
		newv = NULL;
		next = NULL;
		pre = NULL;
	}
	void wirepaint()
	{
		glBegin(GL_LINES);
		Edge* tmp;
		tmp = this->edgelist;
		while (tmp)
		{
			glVertex3f(tmp->lhe->vertex->coord[0], tmp->lhe->vertex->coord[1], tmp->lhe->vertex->coord[2]);	//定义一个点，一般与glBegin()、glEnd()函数连用来绘制图像
			glVertex3f(tmp->rhe->vertex->coord[0], tmp->rhe->vertex->coord[1], tmp->rhe->vertex->coord[2]);
			tmp = tmp->next;
			if (tmp == NULL)
			{
				break;
			}
		}
		glEnd();
	}
	
	void solidpaint()
	{
		Face* tmp;
		Loop* lp;
		GLfloat colors[][3] = { {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
		{0.0,1.0,1.0},{1.0,0.0,1.0},{1.0,1.0,0.0} };
		int Coptions;
		tmp = this->facelist;
		HalfEdge* he;
		Coptions = 0;
		while (tmp)
		{
			glBegin(GL_POLYGON);
			lp = tmp->loopout;
			he = lp->halfedgelist;
			Coptions = Coptions % 6;
			glColor3fv(colors[Coptions]);
			while (he)
			{
				glVertex3f(he->vertex->coord[0], he->vertex->coord[1], he->vertex->coord[2]);
				he = he->next;
				if (he == lp->halfedgelist)
				{
					break;
				}
			}
			Coptions++;
			glEnd();
			tmp = tmp->next;
			if (tmp == this->facelist)
			{
				break;
			}
		}
	}
	Face* facelist;
	Edge* edgelist;
	Vertex* firstv;
	Vertex* newv;
	Solid* next;	
	Solid* pre;	
	int fnum;
	int lnum;
	int vnum;
	int ednum;
};

static Solid* solidlist = NULL;
void  AddToSolidList(Solid* s)
{
	if (solidlist == NULL)
	{
		solidlist = s;
	}
	else
	{
		s->next = solidlist;
		s->pre = solidlist->pre;
		solidlist->pre->next = s;
		solidlist->pre = s;
	}
}
#endif


