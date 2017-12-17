#ifndef ___VERTEX_HEADER__
#define ___VERTEX_HEADER__

#include "common.h"
#include "edge.h"

/*
newVertex : (Graph *, VData *) -> (Vertex)
getVData : (Graph *, Vertex) -> (VData *)
getVertexOf : (Graph *, VData *) -> (Vertex)
getIncidentE : (Graph *, Vertex) -> (Edge List)
getAdjacentV : (Graph *, Vertex) -> (Vertex List)
*/
typedef struct _vertex_interface vertex;
typedef struct _vertexdata_interface vertexdata;
typedef struct _edge_data_interface edgedata;
typedef struct _edge_interface edge;

struct _vertexdata_interface{
	vertex * (*getVertex)(vertexdata*);
	vertex *v;
	char name[4];
};

struct _vertex_interface{
	vertexdata * (*getVertexData)(vertex*);
	edge** (*getIncidentEdges)(vertex*);
	vertex** (*getAdjacentVertices)(vertex*);
  	//void * (*inorder) (BSTree *, void*, void* (*f)(void*, Element));
	//void * (*preorder) (BSTree *, void*, void* (*f)(void*, Element));
	//void * (*postorder) (BSTree *, void*, void* (*f)(void*, Element));
	void (*addIncidentEdge)(vertex*, edge*);
	void (*addAdjacentVertex)(vertex*, vertex*);
	void (*printEdges)(vertex*);
	void (*sortEdges)(vertex*);
	void (*destroy)(vertex*);
	edge** incidentedges;
	int incidentedgesize;
	vertex** adjacentvertices;
	int adjacentvertexsize;
	vertexdata* data;
	
	char visited;
	edge *bestflight;
	int bestcost;
	int arrivaltime;
};

vertex *newVertex(vertexdata*);
vertexdata *newVertexData();
vertex *getVertex(vertexdata*);
vertexdata *getVertexData(vertex*);
edge** getIncidentEdges(vertex*);
vertex** getAdjacentVertices(vertex*);
void addIncidentEdge(vertex*, edge*);
void addAdjacentVertex(vertex*, vertex*);
void printEdges(vertex*);
void sortEdges(vertex*);
void destroyVertex(vertex*);

#endif
