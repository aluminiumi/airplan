#ifndef ___APQ_HEADER__
#define ___APQ_HEADER__

#include "common.h"
#include "edge.h"
#include "vertex.h"

/*
1. enqueue : APQ ×Element ×Priority → APQ ×ElementHandle
2. dequeue : APQ → Element? × APQ
3. peek : APQ → Element?
4. size : APQ → Integer
5. destroy : APQ → ()
6. update : APQ × ElementHandle × Priority → APQ

Properties
1. Elements are removed in order of highest priority, order at
same priority does not matter.
2. An Element’s priority can be updated while on the Queue,
changing the order of dequeuing

1. A Heap Tree is a binary tree filled by level from left to right.
2. Each node on the tree has an associated index.
2.1 The root node has index 1.
2.2 If the parent has index i
2.2.1 the left child has index 2*i
2.2.2 the right child has index 2*i+1.
3. Each node on the tree stores an entry,
the pair (priority, element).
4. The priority of a parent node must be ≥ the priority of both
children.
5. The order of priority between siblings does not matter
*/

typedef int priority;
/*typedef struct _priority_interface priority;
struct _priority_interface {
	int i;
};*/

typedef vertex* element;
/*typedef struct _element_interface element;
struct _element_interface {
	vertex *v;
};*/

typedef int elementhandle;
/*typedef struct _elementhandle_interface elementhandle;
struct _elementhandle_interface {
	int i;
};*/

typedef struct _entry_interface entry;
struct _entry_interface{
	priority p;
	element e;
	elementhandle h;
};

typedef struct _apq_interface apq;
struct _apq_interface{
	void (*destroy)(apq*);
	apq* (*update)(apq*, elementhandle, priority);
	int (*size)(apq*);
	element (*peek)(apq*);
	element (*dequeue)(apq*);
	elementhandle* (*enqueue)(apq*, element, priority);
	entry **entries;
	//elementhandle **elementhandles;
	int s; //size of array
	int maxsize;	
};

apq* createAPQ();
void destroy(apq*);
apq* update(apq*, elementhandle, priority);
int size(apq*);
element peek(apq*);
element dequeue(apq*);
elementhandle* enqueue(apq*, element, priority);

#endif
