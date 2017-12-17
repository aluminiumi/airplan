#include "vertex.h"

vertex *newVertex(vertexdata *vd) {
	//printf("newvertex called\n");
	vertex *v = (vertex*)MALLOC(sizeof(vertex));
	//printf("vertex for %s created at address %p\n", vd->name, v);
	v->getVertexData = getVertexData;
	v->getIncidentEdges = getIncidentEdges;
	v->getAdjacentVertices = getAdjacentVertices;
	v->printEdges = printEdges;
	v->sortEdges = sortEdges;
	v->addIncidentEdge = addIncidentEdge;
	v->addAdjacentVertex = addAdjacentVertex;
	v->destroy = destroyVertex;
	v->incidentedges = MALLOC(sizeof(edge*)*100);
	//printf("incidentedges created at %p\n", v->incidentedges);
	v->adjacentvertices = MALLOC(sizeof(vertex*)*100);
	//printf("adjacentvertices created at %p\n", v->adjacentvertices);
	v->incidentedgesize = 0;
	v->adjacentvertexsize = 0;
	v->data = vd;

	v->visited = 0;
	v->bestflight = NULL;
	v->bestcost = MAX_INT;
	v->arrivaltime = MAX_INT;
	return (vertex*)v;
}

vertexdata *newVertexData(char *name) {
	//printf("newvertexdata called with name %s\n", name);
	vertexdata *vd = (vertexdata*)MALLOC(sizeof(vertexdata));
	//printf("vertexdata for %s created at address %p\n", name, vd);
	strcpy(vd->name, name);
	vd->getVertex = getVertex;
	vd->v = NULL;
	return (vertexdata*)vd;
}

vertex *getVertex(vertexdata* vd) {
	//printf("getvertex called\n");
	//if(vd->v == NULL)
	//	printf("getvertex: returning NULL\n");
	//else
	//	printf("getvertex: found vertex of %s at %p\n", vd->name, vd->v);
	return vd->v;
}

vertexdata *getVertexData(vertex* v) {
	//printf("getvertexdata called\n");
	return v->data;
}

edge** getIncidentEdges(vertex* v) {
	//printf("getincidentedges called\n");
	return v->incidentedges;
}

vertex** getAdjacentVertices(vertex* v) {
	//printf("getadjacentvertices called\n");
	return v->adjacentvertices;
}

void sortEdges(vertex *v) {
	//sort edges and adjacent vertices
	//printf("sortedges called\n");
	//assume only the last element is unsorted

	/*• Print the flight data annotating each edge in the following order:
	1. alphabetic order of target airport codes.
	2. earliest start time
	3. earliest end time*/

	if(v->incidentedgesize > 1) {
		int c=v->incidentedgesize-1;
		int i;
		edge *newedge;
		edge *edge2;
		for(i=v->incidentedgesize-2; i>=0; i--) {
			//printf("c is %d and i is %d\n", c, i);
			newedge = v->incidentedges[c];
			edge2 = v->incidentedges[i];
			if(strcmp(newedge->target->data->name, edge2->target->data->name) < 0) {
				//printf("swapping\n");
				edge *temp = v->incidentedges[i];
				v->incidentedges[i] = v->incidentedges[c];
				v->incidentedges[c] = temp;
				c--;
			} else if(strcmp(newedge->target->data->name, edge2->target->data->name) == 0) {
				//target names match, compare on departure time
				edgedata *newed = newedge->data;
				edgedata *ed2 = edge2->data;
				if(newed->starttime < ed2->starttime) {	
					edge *temp = v->incidentedges[i];
					v->incidentedges[i] = v->incidentedges[c];
					v->incidentedges[c] = temp;
					c--;
				} else if(newed->starttime == ed2->starttime) {
					//departure times match, compare on arrival times
					if(newed->endtime < ed2->endtime) {
						edge *temp = v->incidentedges[i];
						v->incidentedges[i] = v->incidentedges[c];
						v->incidentedges[c] = temp;
						c--;
					} else if(newed->endtime == ed2->endtime) {
						if(newed->cost < ed2->cost) {
							edge *temp = v->incidentedges[i];
							v->incidentedges[i] = v->incidentedges[c];
							v->incidentedges[c] = temp;
							c--;
						}
					}
				}
			}
		}
	}
}

void printEdges(vertex *v) {
	//printf("printEdges called\n");
	int x;
	edge *e;
	edgedata *ed;
	if(v->incidentedgesize == 0)
		printf("-- No Flights\n");
	else {
		for(x=0; x<v->incidentedgesize; x++) {
			e=v->incidentedges[x];
			ed=e->data;
			printf("-- Flight to %s start:%d end:%d cost:%d\n", e->target->data->name, ed->starttime, ed->endtime, ed->cost);
		}
	}
}

void addIncidentEdge(vertex *v, edge *e) {
	//printf("addincidentedge called\n");
	v->incidentedges[v->incidentedgesize] = e;
	v->incidentedgesize++;
	v->addAdjacentVertex(v, e->target);
	v->sortEdges(v);
}

void addAdjacentVertex(vertex *v, vertex *target) {
	//printf("addadjacentvertex called\n");
	v->adjacentvertices[v->adjacentvertexsize] = target;
	v->adjacentvertexsize++;
}

void destroyVertex(vertex *v) {
	//printf("destroyvertex called\n");
	//if(v != NULL && v->data != NULL)
	//	printf("destroying vertex %s\n", v->data->name);
	FREE(v->data);
	FREE(v->incidentedges);
	FREE(v->adjacentvertices);
	FREE(v);
}
