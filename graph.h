#ifndef ___GRAPH_HEADER__
#define ___GRAPH_HEADER__

#include "common.h"
#include "edge.h"
#include "vertex.h"

/*
createGraph : () -> (Graph *)
destroyGraph : (Graph *) -> ()
allEdges : (Graph *) -> (Edge List)
allVertices : (Graph *) -> (Vertex List)

local stuff:
newVertex : (Graph *, VData *) -> (Vertex)
getVData : (Graph *, Vertex) -> (VData *)
getVertexOf : (Graph *, VData *) -> (Vertex)
newEdge : (Graph *, EData *, Vertex, Vertex) -> (Edge)
getEData : (Graph *, Edge) -> (EData *)
getEdgeOf : (Graph *, EData *) -> (Edge)
getSource : (Graph *, Edge) -> (Vertex)
getTarget : (Graph *, Edge) -> (Vertex)
getIncidentE : (Graph *, Vertex) -> (Edge List)
getAdjacentV : (Graph *, Vertex) -> (Vertex List)
*/

typedef struct _graph_interface graph;
struct _graph_interface{
	void (*destroy)(graph*);
	edge** (*getAllEdges)(graph*);
	vertex** (*getAllVertices)(graph*);
	
	vertex* (*addNewVertex)(graph*, vertexdata*);
	vertexdata* (*getVertexData)(graph*, vertex*);
	vertex* (*getVertexByName)(graph*, char*);
	vertexdata* (*getVertexDataByName)(graph*, char*);
	vertex* (*getVertexOf)(graph*, vertexdata*);
	graph* (*addNewEdge)(graph*, edgedata*, vertex*, vertex*);
	edgedata* (*getEdgeData)(graph*, edge*);
	edge* (*getEdgeOf)(graph*, edgedata*);
	vertex* (*getSource)(graph*, edge*);
	vertex* (*getTarget)(graph*, edge*);
	edge** (*getIncidentEdges)(graph*, vertex*);
	vertex** (*getAdjacentVertices)(graph*, vertex*);
	void (*printEdgesAtVertex)(graph*, vertex*);
	void (*sortVertexList)(graph*);

	vertex **vertexlist;
	int vertexlistsize;
	edge **edgelist;
	int edgelistsize;
};

graph *createGraph();
void destroyGraph(graph*);
edge **getAllEdges(graph*);
vertex **getAllVertices(graph*);

vertex *addNewVertex(graph*, vertexdata*);
vertexdata *g_getVertexData(graph*, vertex*);
vertex *getVertexByName(graph*, char*);
vertexdata *getVertexDataByName(graph*, char*);
vertex *getVertexOf(graph*, vertexdata*);
graph *addNewEdge(graph*, edgedata*, vertex*, vertex*);
edgedata *g_getEdgeData(graph*, edge*);
edge *g_getEdgeOf(graph*, edgedata*);
vertex *g_getSource(graph*, edge*);
vertex *g_getTarget(graph*, edge*);
edge **g_getIncidentEdges(graph*, vertex*);
vertex **g_getAdjacentVertices(graph*, vertex*);
void printEdgesAtVertex(graph*, vertex*);
void sortVertexList(graph*);

#endif
