#include "apq.h"

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

printQueue(apq *a) {
	int x;
	for(x=1; x<=a->s; x++) {
		printf("%d:%s:%d:%d ", x, a->entries[x]->e->data->name, a->entries[x]->p, a->entries[x]->h);
	}
	printf("\n");
}

void entryswap(apq *a, entry *entrya, entry *entryb) {
	//printf("entryswap called\n");
	//printf("before swap: ");
	//printQueue(a);
	//printf("swap: entrya was %s:%d\n", entrya->e->data->name, entrya->p);
	//printf("swap: entryb was %s:%d\n", entryb->e->data->name, entryb->p);
	entry temp; // = MALLOC(sizeof(entry));
	temp = *entrya;
	*entrya = *entryb;
	*entryb = temp;
	//FREE(temp);
	//printf("swap: entrya is now %s:%d\n", entrya->e->data->name, entrya->p);
	//printf("swap: entryb is now %s:%d\n", entryb->e->data->name, entryb->p);
	
	
	//4. update elementhandle for A with new location
	//5. update the elementhandle for B with new location
	elementhandle temph = entrya->h;
	entrya->h = entryb->h;
	entryb->h = temph;
	//printf("entrya->h is %d\n", entrya->h);
	//printf("entryb->h is %d\n", entryb->h);
	//printf("temph is %d\n", temph);
	//printf("after swap: ");
	//printQueue(a);
}


void downheap(apq *a, elementhandle h) { 
	//printf("downheap called for %s\n", a->entries[h]->e->data->name);
	//printQueue(a);
	if(h < 1 || h > a->s) {
		return; //do nothing
	}
	/*
	2.2 If the parent has index i
	2.2.1 the left child has index 2*i
	2.2.2 the right child has index 2*i+1.
	*/
	//let greaterchild be the index of the child entry of h->i with greater priority
	if(2*h > a->s)
		return;
	elementhandle leftchild;
	elementhandle rightchild;
	elementhandle greaterchild;
	if(2*h+1 > a->s)
		greaterchild = 2*h;
	else {
		leftchild = 2*h;
		rightchild = 2*h+1;
		if(a->entries[leftchild]->p < a->entries[rightchild]->p)
			greaterchild = leftchild;
		else
			greaterchild = rightchild;
	}
	
	//if a[greaterchild] priority > than a[h->i]’s priority,
		//swap entries
		//downheap(a, k)
	//printf("downheap: checking if greaterchild priority %d < priority %d\n", a->entries[greaterchild]->p, a->entries[h]->p);
	if(a->entries[greaterchild]->p < a->entries[h]->p) {
		//printf("downheap: before calling swap: a is %s and b is %s\n", a->entries[greaterchild]->e->data->name, a->entries[h]->e->data->name); 
		entryswap(a, a->entries[greaterchild], a->entries[h]);
		//printf("downheap: after calling swap: a is %s and b is %s\n", a->entries[greaterchild]->e->data->name, a->entries[h]->e->data->name); 
		downheap(a, greaterchild);
	}
	//printf("downheap: after downheap: ");
	//printQueue(a);
}


void upheap(apq *a, elementhandle h) {
	//printf("upheap called for %s\n", a->entries[h]->e->data->name);
	//printQueue(a);
	if(h <= 1 || h > a->s)
		return;
	//2. if the entry A[i] has greater priority than the entry A[i/2], swap entries.
	if(a->entries[h]->p < a->entries[(h/2)]->p) {	
		//printf("upheap: before calling swap: a is %s and b is %s\n", a->entries[h]->e->data->name, a->entries[h/2]->e->data->name); 
		entryswap(a, a->entries[h], a->entries[(h/2)]);
		//printf("upheap: after calling swap: a is %s and b is %s\n", a->entries[h]->e->data->name, a->entries[h/2]->e->data->name); 

	}
	elementhandle nexth;
	nexth = h/2;
	upheap(a, nexth);
	//printf("after upheap: ");
	//printQueue(a);
}

apq* createAPQ() {
	apq *a = MALLOC(sizeof(apq));
	//printf("created apq at %p\n", a);
	a->entries = MALLOC(sizeof(entry*)*100);
	//printf("created entriesarray at %p\n", a->entries);
	a->destroy = destroy;
	a->update = update;
	a->size = size;
	a->peek = peek;
	a->dequeue = dequeue;
	a->enqueue = enqueue;
	a->s = 0;
	a->maxsize = 0;
}

void destroy(apq *a) {
	//free every entry in entries
	//int x;
	//for(x=0; x<a->maxsize; x++)
	//	if(a->entries[x] != NULL)
	//		FREE(a->entries[x]);
	element e;
	while(a->s > 0) {
		e = dequeue(a);
	}
	//free entries
	FREE(a->entries);
	//free a
	FREE(a);
}

apq* update(apq *a, elementhandle h, priority p) {
	/*
	Update(APQ A, ElementHandle H, Priority P)
	1. Use the element handle to get the node or index of the
	element
	2. change the priority of the element to P
	3. if the priority went up, upheap the updated entry
	4. if the priority went down, downheap the updated entry
	*/
	//printf("update called on %s with handle %d when a->s is %d\n", a->entries[h]->e->data->name, h, a->s);
	//printQueue(a);
	entry *e = a->entries[h];
	priority tp = e->p;
	//printf("update: old priority was %d, new priority is %d\n", e->p, p);
	e->p = p;
	if(e->p < tp) { //priority increased
		//printf("update: upheaping\n");
		upheap(a, h);
	}
	else if(e->p > tp) { //priority decreased
		//printf("update: downheaping\n");
		downheap(a, h);
	}
	//printf("update: after update: ");
	//printQueue(a);
}

int size(apq *a) {
	return a->s;
}

element peek(apq *a) {
	if(a->s > 0)
		return a->entries[1]->e;
	return NULL;
}

element dequeue(apq *a) {
	//printf("dequeue called\n");
	//printQueue(a);
	if(a->s < 1)
		return NULL;
	//When removing an entry with the highest priority.
	//1. The element at the root is saved to be returned.
	element root = a->entries[1]->e;
	entry *rootentry = a->entries[1];
	//2. If there are n nodes on the tree, the entry of the node with
	//index n is moved to the root,
	a->entries[1] = a->entries[a->s];
	//3. The node at index n is deleted
	//a->entries[a->s] = NULL;
	a->s--;
	int x;
	for(x=1; x<=a->s; x++) {
		a->entries[x]->h = x;	
	}
	//4. The downheap operation is recursively called on the entry in
	//the root to fix the parent-child priority order.
	elementhandle nexth;
	nexth = 1;
	downheap(a, nexth);
	FREE(rootentry);
	//5. The element that was save is now returned.
	return root;
}

elementhandle *enqueue(apq *a, element e, priority p) {
	//When inserting a new entry (priority, element) on the heap tree,
	//1. If there are n nodes on the tree, a new node is created at
	//index n+1.
	//printf("enqueue called for %s\n", e->data->name);
	//printQueue(a);
	a->s++;
	//if(a->s > a->maxsize)
	//	a->maxsize++;
	entry *newentry = MALLOC(sizeof(entry));
	//printf("created entry at %p\n", newentry);
	newentry->e = e;
	newentry->p = p;
	newentry->h = a->s;
	a->entries[a->s] = newentry;

	//2. The entry is initially written to the new node.
	
	//3. The upheap operation is recursively called on the entry to fix
	//the parent-child priority order.
	upheap(a, newentry->h);
	
	//elementhandle *h = MALLOC(sizeof(elementhandle));
	//*h = a->s;
	//printf("enqueue: returning handle %d for %s\n", newentry->h, e->data->name);
	return &newentry->h;
}
