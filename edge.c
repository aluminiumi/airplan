#include "edge.h"

edge *newEdge(edgedata *ed, vertex *source, vertex *target) {
	//printf("newedge called\n");
	edge *e = (edge*)MALLOC(sizeof(edge));
	//printf("edge created at address %p\n", e);
	e->getEdgeData = getEdgeData;
	e->getSource = getSource;
	e->getTarget = getTarget;
	e->destroy = destroyEdge;
	e->source = source;
	e->target = target;
	e->data = ed;
	return (edge*) e;
}

edgedata *newEdgeData(int starttime, int endtime, int cost) {
	//printf("newedgedata called\n");
	edgedata *ed = (edgedata*)MALLOC(sizeof(edgedata));
	//printf("edgedata created at address %p\n", ed);
	ed->getEdge = getEdge;
	ed->starttime = starttime;
	ed->endtime = endtime;
	ed->cost = cost;
	ed->e = NULL;
	return ed;
}

edge * getEdge(edgedata *ed) {
	//printf("getedge called\n");
	return ed->e;
}

edgedata * getEdgeData(edge *e) {
	//printf("getedgedata called\n");
	return e->data;
}

vertex * getSource(edge *e) {
	//printf("getsource called\n");
	return e->source;
}

vertex * getTarget(edge *e) {
	//printf("gettarget called\n");
	return e->target;
}

void destroyEdge(edge *e) {
	//printf("destroyedge called\n");
	FREE(e->data);
	FREE(e);
}

