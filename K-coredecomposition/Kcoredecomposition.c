/*
Adel EL AMRAOUI
March 2020
to compile:gcc -o Kcoredecomposition Kcoredecomposition.c adjarray.c
to execute:./Kcoredecomposition edgelist.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>//to estimate the runing time
#include<limits.h>
#include "adjarray.h"
#define NBITER 3
/*Structures*/
typedef struct {
	unsigned long * heap;
	unsigned long capacity;
	unsigned long size;
	//todo: indiceK
	//unsigned long * indiceK
} minHeap;

unsigned long max(unsigned long a,unsigned long b ){
	return (a>b)?a:b;
}

int parent(int i){
	return (i-1)/2;
}
int left(int i) { return (2*i + 1); }
int right(int i) { return (2*i + 2); }

int degree(unsigned long * cd, unsigned long noeud){
	return cd[noeud+1]-cd[noeud];
}

minHeap* makeMinheap(unsigned long capacity){
	minHeap* m=(minHeap*)malloc(sizeof(minHeap));
	m->capacity=capacity;
	m->heap=(unsigned long*)malloc(m->capacity*sizeof(unsigned long));
	m->size=0;
	return m;
}
void insertkey(unsigned long* cd,minHeap* m,unsigned long element){
	unsigned long i;
	unsigned long tmp;
	if(m->size==m->capacity) {
		printf("tas minimum plein, exit \n");
		exit(0);
	}
	m->size++;
	i= m->size-1;
	m->heap[i]=element;

	while(i!=0 && m->heap[degree(cd,parent(i))]>degree(cd,m->heap[i])){
		tmp= m->heap[i];
		m->heap[i]=m->heap[parent(i)];
		m->heap[parent(i)]=tmp;
		i=parent(i);
	}
}

minHeap* makeMinHeapfromG(adjarray*g){
	printf("start makeMinheapfromG\n");
	minHeap* m=makeMinheap(g->n);
	unsigned long i=0;
	//parcours des noeuds de g + ajout dans le tas min
	for(i=0;i<g->n;i++){
		if(degree(g->cd,i)>0){
			insertkey(g->cd,m,i);
		}
	}
	printf("end makeMinheapfromG\n");
	return m;
}

void free_minheap(minHeap*m){
	free(m->heap);
	free(m);
}
void decreaseKey(unsigned long* cd,minHeap * m,unsigned long i, unsigned long new_val){
	m->heap[i]=new_val;
	unsigned long tmp;
	while(i!=0 && m->heap[degree(cd,parent(i))]>degree(cd,m->heap[i])){
		tmp= m->heap[i];
		m->heap[i]=m->heap[parent(i)];
		m->heap[parent(i)]=tmp;
		i=parent(i);
	}
}
void minHeapify(unsigned long * cd,minHeap *m,unsigned long i){
	unsigned long l=left(i);
	unsigned long r= right(i);
	unsigned long smallest=i;
	unsigned long tmp;
	if(l<(m->size) && degree(cd,(m->heap)[l])<degree(cd,(m->heap)[i])){
		smallest=l;
	}
	if(r<(m->size) && degree(cd,(m->heap)[r])<degree(cd,(m->heap)[smallest])){
		smallest=r;
	}
	if(smallest!=i){
		tmp= (m->heap)[i];
		(m->heap)[i]= (m->heap)[smallest];
		(m->heap)[smallest]=tmp;
		minHeapify(cd,m,smallest);
	}
}

//mise à jour des degrees des voisins après une suppression
void updatecd(unsigned long*adj,unsigned long*cd, unsigned long* supprimees,unsigned long n){
	unsigned long i;
	unsigned long v;
	//parcours des voisins du noeud n
	for(i=cd[n];i<cd[n+1];i++){
			v=adj[i];
			if(supprimees[v]!=1) cd[v+1]--;
	}
}
unsigned long extractMin(unsigned long* adj,unsigned long* cd,minHeap * m,unsigned long *supprimees ){
	if(m->size<=0) return LONG_MAX;
	if(m->size==1){
		(m->size)--;
		return m->heap[0];
	}
	unsigned long root= m->heap[0];
	m->heap[0]=m->heap[(m->size)-1];
	(m->size)--;
	//supprimmes[i]=1 : le noeud d'identifiant i a été supprimé
	supprimees[root]=1;
	updatecd(adj,cd,supprimees,root);
	minHeapify(cd,m,0);
	return root;
}

void deleteKey(unsigned long* adj,unsigned long* cd,minHeap * m,unsigned long i,unsigned long *supprimees){
	decreaseKey(cd,m,i,0);
	extractMin(adj,cd,m,supprimees);
}

unsigned long* copycd(unsigned long* cd,unsigned long size){
	unsigned long *res= (unsigned long*)calloc(size,sizeof(unsigned long));
	unsigned long i=0;
	for(int i=0;i<size;i++){
		res[i]=cd[i];
	}
	return res;
}

void printMinheap(minHeap * m){
	unsigned long i;
	if(m->size==1) {
			printf("[%lu]\n",m->heap[0]);
			return;
	}
	for(i=0;i<(m->size);i++){
		if (i==0){
			printf("[%lu,",m->heap[i]);
			continue;
		}

		if(i==(m->size)-1) {
			printf("%lu]\n",m->heap[i]);
			continue;
		}
		printf("%lu,",m->heap[i]);
	}
}

/*ALGORITHMES------------------------------------------------------------------*/
/*procédure: extract du noeud dans le tas
pour chaque voisin:
	mettre à jour le degré
	le faire remonter dans l'abre
*/

unsigned long Kcoredecomposition(adjarray* g,unsigned long* order){
	minHeap* m=	makeMinHeapfromG(g);
	unsigned long* cd= copycd(g->cd,(g->n)+1);
	unsigned long * supp=(unsigned long*)calloc(g->n,sizeof(unsigned long));
	unsigned long i= g->n;
	unsigned long c=0;
	unsigned long v;
	printf("tas min : capacity:=%lu\n",m->capacity);
	printf("tas min : size:=%lu\n",m->size);
	while(m->size>0){
		c= max(c,degree(cd,(m->heap)[0]));
		//printf("avant extractMin: \n");
		//printMinheap(m);
		v=extractMin(g->adj,cd,m,supp);
		//printf("apres extractMin: \n");
		//printMinheap(m);
		order[v]=i;
		i--;
	}

	free(cd);
	free(supp);
	free_minheap(m);
	return c;
}


/*TESTS-----------------------------------------------------------------------------------------------------------*/

void testminHeap(){
	minHeap* m= makeMinheap(4);
	unsigned long* cd= (unsigned long*)calloc(5,sizeof(unsigned long));
	cd[0]=0;
	cd[1]=1;
	cd[2]=3;
	cd[3]=6;
	cd[4]=6;

	printf("insertion du noeud 0 de degre 1\n");
	insertkey(cd,m,0);
	printMinheap(m);
	printf("insertion du noeud 1 de degre 2\n");
	insertkey(cd,m,1);
	printMinheap(m);
	printf("insertion du noeud 2 de degre 3 \n");
	insertkey(cd,m,2);
	printMinheap(m);
	printf("insertion du noeud 3 de degre 0 \n");
	insertkey(cd,m,3);
	printMinheap(m);
	free(cd);
	free_minheap(m);
}

int main(int argc,char** argv){
	adjarray* g;
	unsigned long i;
	time_t t1,t2;
	unsigned long corevalue;
	unsigned long *coredecomposition= (unsigned long*) calloc(g->n,sizeof(unsigned long));

	t1=time(NULL);

	printf("Reading adjArray from file %s\n",argv[1]);
	g=read_adjarray(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency array\n");
	mkadjarray(g);
	printf("adjacency array is ready\n");
	corevalue=Kcoredecomposition(g,coredecomposition);
	printf("corevalue= %lu\n",corevalue);
	printf("K-core decomposition:\n");
	/*for(i=0;i<g->n;i++){
		printf("order[%lu]=%lu\n",i,coredecomposition[i]);
	}*/
	//testminHeap();
	free(coredecomposition);
	free_adjarray(g);
	t2=time(NULL);
	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	return 0;
}
