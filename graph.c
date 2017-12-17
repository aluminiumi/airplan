#include "graph.h"

graph * createGraph() {
	//printf("newgraph called\n");
	graph *g = (graph*)MALLOC(sizeof(graph));
	//printf("graph created at address %p\n", g);
	g->destroy = destroyGraph;
	g->getAllEdges = getAllEdges;
	g->getAllVertices = getAllVertices;

	g->addNewVertex = addNewVertex;
	g->getVertexData = g_getVertexData;
	g->getVertexByName = getVertexByName;
	g->getVertexDataByName = getVertexDataByName;
	g->getVertexOf = getVertexOf;
	g->addNewEdge = addNewEdge;
	g->getEdgeData = g_getEdgeData;
	g->getEdgeOf = g_getEdgeOf;
	g->getSource = g_getSource;
	g->getTarget = g_getTarget;
	g->getIncidentEdges = g_getIncidentEdges;
	g->getAdjacentVertices = g_getAdjacentVertices;
	g->printEdgesAtVertex = printEdgesAtVertex;
	g->sortVertexList = sortVertexList;

	g->vertexlist = MALLOC(sizeof(vertex*)*100);
	//printf("vertexlist created at address %p\n", g->vertexlist);
	g->vertexlistsize = 0;
	g->edgelist = MALLOC(sizeof(edge*)*100);
	//printf("edgelist created at address %p\n", g->edgelist);
	g->edgelistsize = 0;
	return (graph*) g;
}

edge **getAllEdges(graph *g) {
	//printf("getalledges called\n");
	return g->edgelist;
}
vertex **getAllVertices(graph *g) {
	//printf("getallvertices called\n");
	return g->vertexlist;
}

vertex *addNewVertex(graph *g, vertexdata *vd) {
	//printf("addnewvertex called\n");
	vertex *v = (vertex*)newVertex(vd);
	vd->v = v;
	g->vertexlist[g->vertexlistsize] = v;
	g->vertexlistsize++;
	g->sortVertexList(g);
	return v;
}

vertexdata *g_getVertexData(graph *g, vertex *v) {
	//printf("getvertexdata called\n");
	return v->getVertexData(v);
}

vertex *getVertexByName(graph *g, char *name) {
	vertexdata *vd = g->getVertexDataByName(g, name);
	if(vd == NULL)
		return NULL;
	return vd->v;
}

vertexdata *getVertexDataByName(graph *g, char *name) {
	//printf("getvertexdatabyname called\n");
	//printf("searching for vertex by name %s\n", name);
	if(g->vertexlistsize == 0) {
	//	printf("gvdbn: no vertexes, returning NULL\n");
		return NULL;
	}
	int x;
	for(x=0; x<g->vertexlistsize; x++) {
		if(strcmp(name, g->vertexlist[x]->data->name) == 0) {
	//		printf("gvdbn: match for %s, returning vertexdata\n", name);
			return g->vertexlist[x]->data;
		}
	}
	//printf("gvdbn: no match for %s, returning NULL\n", name);
	return NULL;
}

vertex *getVertexOf(graph *g, vertexdata *vd) {
	//printf("getvertexof called\n");
	return vd->getVertex(vd);
}

graph *addNewEdge(graph *g, edgedata *ed, vertex *source, vertex *target) {
	//printf("addnewedge called\n");
	edge *e = (edge*)newEdge(ed, source, target);
	ed->e = e;
	g->edgelist[g->edgelistsize] = e;
	g->edgelistsize++;
	source->addIncidentEdge(source, e);
	//printf("edge created from %s to %s\n", source->data->name, target->data->name);
	return g;
}

edgedata *g_getEdgeData(graph *g, edge *e) {
	//printf("getedgedata called\n");
	return e->getEdgeData(e);
}

edge *g_getEdgeOf(graph *g, edgedata *ed) {
	//printf("getedgeof called\n");
	return ed->getEdge(ed);
}

vertex *g_getSource(graph *g, edge *e) {
	//printf("getsource called\n");
	return e->getSource(e);
}

vertex *g_getTarget(graph *g, edge *e) {
	//printf("gettarget called\n");
	return e->getTarget(e);
}

edge **g_getIncidentEdges(graph *g, vertex *v) {
	//printf("getincidentedges called\n");
	return v->getIncidentEdges(v);
}

vertex **g_getAdjacentVertices(graph *g, vertex *v) {
	//printf("getadjvertices called\n");
	return v->getAdjacentVertices(v);
}

void printEdgesAtVertex(graph *g, vertex *v) {
	//printf("printedgesatvertex called\n");
	v->printEdges(v);
}

void sortVertexList(graph* g) {
	//printf("sortvertexlist called\n");
	//assume only the last element is unsorted
	if(g->vertexlistsize > 1) {
		int c=g->vertexlistsize-1;
		int i;
		for(i=g->vertexlistsize-2; i>=0; i--) {
			//printf("c is %d and i is %d\n", c, i);
			//printf("vertexsort: comparing %s and %s\n", g->vertexlist[c]->data->name, g->vertexlist[i]->data->name);
			if(strcmp(g->vertexlist[c]->data->name, g->vertexlist[i]->data->name) < 0) {
				//printf("swapping\n");
				vertex *temp = g->vertexlist[i];
				g->vertexlist[i] = g->vertexlist[c];
				g->vertexlist[c] = temp;
				c--;
			}	
		}
	}
}

void destroyGraph(graph *g) {
	//printf("destroygraph called\n");
	int x;
	//for vertex in vertexlist, destroy
	//printf("destroying %d vertexes\n", g->vertexlistsize);
	for(x=0; x < g->vertexlistsize; x++)
		g->vertexlist[x]->destroy(g->vertexlist[x]);
	FREE(g->vertexlist);
	
	//for edge in edgelist, destroy
	//printf("destroying %d edges\n", g->edgelistsize);
	for(x=0; x < g->edgelistsize; x++)
		g->edgelist[x]->destroy(g->edgelist[x]);
	FREE(g->edgelist);
	FREE(g);
}
