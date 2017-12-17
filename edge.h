#ifndef ___EDGE_HEADER__
#define ___EDGE_HEADER__

#include "common.h"
#include "vertex.h"

/*
newEdge : (Graph *, EData *, Vertex, Vertex) -> (Edge)
getEData : (Graph *, Edge) -> (EData *)
getEdgeOf : (Graph *, EData *) -> (Edge)
getSource : (Graph *, Edge) -> (Vertex)
getTarget : (Graph *, Edge) -> (Vertex)
*/
typedef struct _vertexdata_interface vertexdata;
typedef struct _vertex_interface vertex;
typedef struct _edge_data_interface edgedata;
typedef struct _edge_interface edge;

struct _edge_data_interface{
	edge * (*getEdge)(edgedata*);
	edge *e;
	int starttime;
	int endtime;
	int cost;	
};

struct _edge_interface{
	edgedata * (*getEdgeData)(edge *);
	vertex * (*getSource)(edge *);
	vertex * (*getTarget)(edge *);
	//void * (*inorder) (BSTree *, void*, void* (*f)(void*, Element));
	//void * (*preorder) (BSTree *, void*, void* (*f)(void*, Element));
	//void * (*postorder) (BSTree *, void*, void* (*f)(void*, Element));
	void (*destroy)(edge*);
	vertex *source;
	vertex *target;
	edgedata *data;
};

edge *newEdge(edgedata*, vertex*, vertex*);
edgedata *newEdgeData(int, int, int);
edge * getEdge(edgedata*);
edgedata * getEdgeData(edge*);
vertex * getSource(edge*);
vertex * getTarget(edge*);
void destroyEdge(edge*);

#endif
