#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"apq.h"
#include"graph.h"
#include"trackedmemory.h"

void printBestFlight(vertex *q){
	if(q->bestflight == NULL) 
		return;
	printBestFlight(q->bestflight->source);	
	edge *z = q->bestflight;
	printf("Flight %s %s start:%d end:%d cost:%d \n", z->source->data->name, z->target->data->name, z->data->starttime, z->data->endtime, z->data->cost);
}

elementhandle *vertexToHandle(graph *g, apq *a, elementhandle **eharray, vertex *v) {
	//printf("vertextohandle called\n");
	int x;
	for(x=1; x<=a->s; x++) {
		if(v == a->entries[x]->e)
			return &a->entries[x]->h;
	}
	/*for(x=0; x<g->vertexlistsize; x++) {
		if(v == g->vertexlist[x]) {
			printf("vertexToHandle: returning %d\n", *eharray[x]);
			return (elementhandle*)eharray[x];
		}
	}*/
	//printf("vertextohandle: returning null\n");
	return NULL;
}

void dijkstra(graph *g, char *source, char *dest, char flag) {
	//flag: 0=cheapest, 1=fastest
	int x;
	vertex *sv = g->getVertexByName(g, source);
	if(sv == NULL) {
		//printf("Airport %s Not Found\n", source);
		return;
	}
	sv->arrivaltime = 0;
	sv->bestcost = 0;
	vertex *dv = g->getVertexByName(g, dest);
	if(dv == NULL) {
		//printf("Airport %s Not Found\n", dest);
		return;
	}

	apq *a = (apq*)createAPQ();
	//vertex **vlcopy = MALLOC(sizeof(g->vertexlist));
	int vcount = g->vertexlistsize;
	//for(x=0; x<vcount; x++) {
	//	vlcopy[x] = g->vertexlist[x];
	//}
	vertex **vlcopy = g->vertexlist;
	
	elementhandle **handlearray = MALLOC(sizeof(elementhandle*)*100);
	//1. Add the start vertex to the APQ with priority/cost/endtime = 0
	//element se = sv;
	//priority sp = 0;
	//printf("enqueueing source\n");
	
	//2. Add every other vertex to the APQ with priority/cost/endtime = MAX integer value
	for(x=0; x<vcount; x++) {
		if(strcmp(vlcopy[x]->data->name, source) != 0) {
			//printf("enqueueing node\n");
			handlearray[x] = a->enqueue(a, vlcopy[x], MAX_INT); 
		} else
			handlearray[x] = a->enqueue(a, sv, 0);
	}
	
	//3. Each vertex should be able have the following:
	//(a) mark for visitation
	//(b) best flight (edge)
	//(c) best cost:
		//i. Cheapest: sum of costs of flights to target vertex
		//ii. Earliest: Arrival time of flight used to get to target vertex

	//4. Repeat Until APQ is empty or until end vertex is removed from APQ
	int i=1;
	while(a->peek(a) != NULL) {
		//(a) Let V = The vertex with least cost/endtime by dequeueing from APQ
		vertex *V = (vertex*)dequeue(a);
		//printf("pulled %s off queue\n", V->data->name);
		//(b) mark V as visited.
		V->visited = 1;
		if(strcmp(V->data->name, dest) == 0) {
			//printf("got dest vertex; breaking\n");
			break;
		}
		//(c) For each edge E= (V,V2) where V is the source vertex
		for(x=0; x<V->incidentedgesize; x++) {
			edge *e = V->incidentedges[x];
			vertex *V2 = e->target;
			//printf("looking at edge from %s to %s\n", e->source->data->name, e->target->data->name);
			//V2->arrivaltime = e->data->endtime;
			//i. If the StartTime(E) ≤ Arrival(V) skip the edge.
			if(e->data->starttime <= V->arrivaltime) {
				//printf("skipping, late arrival\n");
			} else {
			//ii. Otherwise check if better cost/earlier path found
				if(flag == 0) {
				//A. For Cheapest: Cost(V) + Cost(E) ≤ Cost(V2)
					//printf("comparing %d and %d\n", V->bestcost + e->data->cost, V2->bestcost);
					if(V->bestcost + e->data->cost <= V2->bestcost) {
						//printf("better cost found, update %s's data\n", V2->data->name);
						V2->bestcost = V->bestcost + e->data->cost;
						V2->arrivaltime = e->data->endtime;
						V2->bestflight = e;
						elementhandle *vh = vertexToHandle(g, a, handlearray, V2);
						a->update(a, *vh, V2->bestcost);
						//a->update(a, i, V2->bestcost);
					} else if(V->bestcost + e->data->cost == V2->bestcost) {
						//if(e->data->endtime < V2->arrivaltime) {
						if(e->data->starttime > V->arrivaltime && e->data->endtime < V2->arrivaltime) {
							V2->bestcost = V->bestcost + e->data->cost;
							//V2->bestcost = e->data->endtime;
							V2->arrivaltime = e->data->endtime;
							V2->bestflight = e;
							elementhandle *vh = vertexToHandle(g, a, handlearray, V2);
							a->update(a, *vh, V2->bestcost);
						}
					}
				} else {
				//B. For Earliest: StartTime(E) > Arrival(V) AND Arrival(E) < Arrival(V2)
					//printf("comparing new arrival time %d and old arrival time %d\n", e->data->endtime, V2->arrivaltime);
					//StartTime(E) > Arrival(V) AND Arrival(E) < Arrival(V2)
					//if(e->data->endtime < V2->arrivaltime) {
					if(e->data->endtime < V2->arrivaltime) {
						//printf("better cost found, update %s's data\n", V2->data->name);
						V2->bestcost = e->data->endtime;
						V2->arrivaltime = e->data->endtime;
						V2->bestflight = e;
						elementhandle *vh = vertexToHandle(g, a, handlearray, V2);
						a->update(a, *vh, V2->bestcost);
						//a->update(a, i, V2->arrivaltime);
					} else if(e->data->endtime == V2->arrivaltime) {
						if(V->bestcost + e->data->cost <= V2->bestcost) {
							/*V2->bestcost = e->data->endtime;
							//V2->bestcost = V->bestcost + e->data->cost;
							V2->arrivaltime = e->data->endtime;
							V2->bestflight = e;
							elementhandle *vh = vertexToHandle(g, a, handlearray, V2);
							a->update(a, *vh, V2->bestcost);
							*/
						}
					}
				}
			}
		}
		i++;
		
	}
	//5. Once the final destination vertex is removed the APQ, the proper itinerary
	//is recovered by walking from the end to the start vertices following the
	//source of the best flights to each vertex along the way.
	vertex *q = dv;
	printBestFlight(q);
	//printf("%s\n", q->data->name);
	//printf("%s\n", dv->data->name);
	//printf("%s\n", sv->data->name);
	//printf("%s\n", dv->bestflight->source->data->name);
	//printf("%s\n", sv->bestflight->source->data->name);
	/*while(q->bestflight != NULL) { // != sv) {
		edge *z = q->bestflight;
		printf("Flight %s %s start:%d end:%d cost:%d\n", z->source->data->name, z->target->data->name, z->data->starttime, z->data->endtime, z->data->cost);
		q = q->bestflight->source;
	}*/
	
	//6. To print the flights in order, you can use recursion to walk the best edges
	//from end to start until the start is reached and print the flight of each
	//edge after the recursive call returns.


	
	/*
	• Use Dijkstra’s Algorithm to compute the trip from APC1 to APC2 which
	minimizes the cost.
	• Print: Cheapest From:<APC1> To:<APC2>
	• Print the flights in temporal order, from start to final end, 1 flight per
	line:
	• Flight <APC> <APC> start:<start time> end:<end time> cost:<Cost>
	*/

	/*
	• Print: Earliest From:<APC1> To:<APC2>
	• Print the flights in temporal order, from start to final end, 1 flight per
	line:
	• Flight <APC> <APC> start:<start time> end:<end time> cost:<Cost>
	*/

	//cleanup
	for(x=0; x<vcount; x++) {
		vlcopy[x]->visited = 0;	
		vlcopy[x]->bestflight = NULL;
		vlcopy[x]->bestcost = MAX_INT;
		vlcopy[x]->arrivaltime = MAX_INT;
		//FREE(handlearray[x]);
	}
	FREE(handlearray);
	a->destroy(a);
}

int main(int argc, char ** argv) {
	char command[20];
	char airport1[4], airport2[4];
	int cost, starttime, endtime;
  
	graph *g = (graph*)createGraph();
	
	while(scanf("%s", command) > 0){
		if(strcmp(command, "List") == 0) {
			//printf("got list\n");
			scanf("%s", airport1);
			if(strcmp(airport1, "*") == 0) {
				//printf("listing all\n");
				if(g->vertexlistsize == 0)
					printf("Network Is Empty\n");
				else {
					int x;
					for(x=0; x<g->vertexlistsize; x++) {
						printf("Flights From %s\n", g->vertexlist[x]->data->name);
						g->printEdgesAtVertex(g, g->vertexlist[x]);
					}	
				}

			} else {
				//printf("checking for airport %s\n", airport1);
				vertex *v = g->getVertexByName(g, airport1);
				if(v == NULL)
					printf("Airport %s Not Found\n", airport1);
				else {
					printf("Flights From %s\n", airport1);
					g->printEdgesAtVertex(g, v);
				}
			}
		} else if(strcmp(command, "Flight") == 0) {
			//The Flight <APC> <APC> <Cost> <start time> <end time> command
			scanf("%s", airport1);
			scanf("%s", airport2);
			scanf("%d", &starttime);
			scanf("%d", &endtime);
			scanf("%d", &cost);

			//get the vertex data for each airport code, or create it
			vertexdata *firstvertexdata = (vertexdata*)g->getVertexDataByName(g, airport1);
			if(firstvertexdata == NULL)
				firstvertexdata = (vertexdata*)newVertexData(airport1);
			vertexdata *secondvertexdata = (vertexdata*)g->getVertexDataByName(g, airport2);
			if(secondvertexdata == NULL)
				secondvertexdata = (vertexdata*)newVertexData(airport2);
			
			//Get the vertices for the airport codes, or create them
			vertex *vertex1 = (vertex*)g->getVertexOf(g, firstvertexdata);
			if(vertex1 == NULL)
				vertex1 = (vertex*)g->addNewVertex(g, firstvertexdata);
			vertex *vertex2 = (vertex*)g->getVertexOf(g, secondvertexdata);
			if(vertex2 == NULL)
				vertex2 = (vertex*)g->addNewVertex(g, secondvertexdata);

			//Create a new edge from <APC1> to <APC2>
			edgedata *ed = (edgedata*)newEdgeData(starttime, endtime, cost);
			g->addNewEdge(g, ed, vertex1, vertex2); 
		} else if(strcmp(command, "Cheapest") == 0) {
			scanf("%s", airport1);
			scanf("%s", airport2);
			printf("Cheapest From:%s To:%s\n", airport1, airport2);
			dijkstra(g, airport1, airport2, 0);

		} else if(strcmp(command, "Earliest") == 0) {
			scanf("%s", airport1);
			scanf("%s", airport2);
			printf("Earliest From:%s To:%s\n", airport1, airport2);
			dijkstra(g, airport1, airport2, 1);
		}
	}
	//printf("Freeing memory.\n");
	g->destroy(g);
	
	//printf("Exiting.\n");
	printMemoryUsage();
	assertNoMemoryLeaks();
	return 0;
}

