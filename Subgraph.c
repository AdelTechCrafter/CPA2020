/*
Maximilien Danisch
September 2017
http://bit.ly/danisch
maximilien.danisch@gmail.com
Info:
Feel free to use these lines as you wish. This program loads a graph in main memory.
To compile:
"gcc BFS.c -O9 -o adjarray".
To execute:
"./adjarray edgelist.txt".
"edgelist.txt" should contain the graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
The prograph will load the graph in main memory and then terminate.
Note:
If the graph is directed (and weighted) with selfloops and you want to make it undirected unweighted without selfloops, use the following linux command line.
awk '{if ($1<$2) print $1" "$2;else if ($2<$1) print $2" "$1}' net.txt | sort -n -k1,2 -u > net2.txt
Performance:
Up to 200 million edges on my laptop with 8G of RAM: takes more or less 4G of RAM and 30 seconds (I have an SSD hardrive) for 100M edges.
*/

/*delete self loops:
awk '{if ($1<$2) print $1" "$2;else if ($2<$1) print $2" "$1}' net.txt | sort -n -k1,2 -u > net2.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>//to estimate the runing time
#include<limits.h>

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed
#define BUFFSIZELINE 100
#define NBITER 3
/*Structures*/
typedef struct {
	unsigned long * heap;
	unsigned long capacity;
	unsigned long size;
	//todo: indiceK
	//unsigned long * indiceK
} minHeap;

typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjarray;


/*UTILITAIRES--------------------------------------------------------------------------*/

//compute the maximum of three unsigned long
unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}
unsigned long max(unsigned long a,unsigned long b ){
	(a>b) ? a : b;
}

int parent(int i){
	return (i-1)/2; 
}
int left(int i) { return (2*i + 1); } 
int right(int i) { return (2*i + 2); }

int degree(unsigned long * cd, unsigned long noeud){
	return cd[noeud+1]-cd[noeud];
}
/*CONSTRUCTEURS-----------------------------------------------------------*/

//reading the adjarray from file
adjarray* readadjarray(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");
	//ajout
	char buffer[BUFFSIZELINE];

	adjarray*g=(adjarray*)malloc(sizeof(adjarray));
	g->n=0;
	g->e=0;
	g->edges=(edge*)malloc(e1*sizeof(edge));//allocate some RAM to store edges

	//ajout
	fgets(buffer, BUFFSIZELINE, file);
	fgets(buffer, BUFFSIZELINE, file);
	fgets(buffer, BUFFSIZELINE, file);
	fgets(buffer, BUFFSIZELINE, file);

	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (++(g->e)==e1) {//increase allocated RAM if needed
			e1+=NLINKS;
			g->edges=(edge*)realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;

	g->edges=(edge*)realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//building the adjacency array
void mkadjarray(adjarray* g){
	unsigned long i,u,v;
	unsigned long *d=(unsigned long *)calloc(g->n,sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=(unsigned long *)malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=(unsigned long *)malloc(2*g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(d);
	//free(g->edges);
}

void makeMinheap(unsigned long capacity,minHeap* m){
	m->capacity=capacity;
	m->heap=(unsigned long*)malloc(m->capacity*sizeof(unsigned long));
	m->size=0;
	
}
void insertkey(unsigned long* cd,minHeap* m,unsigned long element){
	unsigned long i;
	unsigned long tmp;
	if(m->size==m->capacity) {
		printf("tas minimum plein");
		return;
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

void makeMinHeapfromG(adjarray*g,minHeap* m){
	int i=0;
	//parcours des noeuds de g + ajout dans le tas min
	for(i=0;i<g->n;i=i+((g->cd)[i+1]-(g->cd)[i])){
		insertkey(g->cd,m,g->adj[i]);
	}
	
}
/*Destructeurs-----------------------------------------------------------------------*/
//freeing memory
void free_adjarray(adjarray*g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g);
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
void updatecd(unsigned long*adj,unsigned long*cd, unsigned int* supprimees,unsigned long n){
	unsigned long i;
	unsigned long v;
	//parcours des voisins du noeud n
	for(i=cd[n];i<cd[n+1];i++){
			v=adj[i];
			if(supprimees[v]!=1) cd[v+1]--;
	}
}
unsigned long extractMin(unsigned long* adj,unsigned long* cd,minHeap * m,unsigned int *supprimees ){
	if(m->size<=0) return LONG_MAX;
	if(m->size==1){
		(m->size)--;
		return m->heap[0];
	}
	int root= m->heap[0];
	m->heap[0]=m->heap[(m->size)-1];
	(m->size)--;
	//supprimmes[i]=1 : le noeud d'identifiant i a été supprimé
	supprimees[root]=1;
	updatecd(adj,cd,supprimees,root);
	minHeapify(cd,m,0);
	return root;
}

void deleteKey(unsigned long* adj,unsigned long* cd,minHeap * m,unsigned long i,unsigned int *supprimees){
	decreaseKey(cd,m,i,0);
	extractMin(adj,cd,m,supprimees);
}









unsigned long* copycd(unsigned long* cd,int size){
	unsigned long *res= (unsigned long*)malloc(size*sizeof(unsigned long));
	int i=0;
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
	minHeap* m=(minHeap*)malloc(sizeof(minHeap));
	makeMinHeapfromG(g,m);
	
	unsigned long* cd= copycd(g->cd,(g->n)+1);
	unsigned int * supp=(unsigned int*)calloc(0,(g->n)*sizeof(unsigned int));
	unsigned long i= g->n;
	unsigned long c=0;
	unsigned long v;
	
	while(m->size>0){
		c= max(c,degree(cd,(m->heap)[0]));
		v=extractMin(g->adj,cd,m,supp);
		order[v]=i;
		i--;
	}
	
	free_minheap(m);
	return c;
	
}


/*TESTS-----------------------------------------------------------------------------------------------------------*/
void testminHeap(){
	minHeap* m= (minHeap*)malloc(sizeof(minHeap));
	unsigned long* cd= (unsigned long*)calloc(5,sizeof(unsigned long));
	cd[0]=0;
	cd[1]=1;
	cd[2]=3;
	cd[3]=6;
	cd[4]=6;
	
	makeMinheap(4,m);
	printf("insertion du noeud 0 de degré 1\n");
	insertkey(cd,m,0);
	printMinheap(m);
	printf("insertion du noeud 1 de degré 2\n");
	insertkey(cd,m,1);
	printMinheap(m);
	printf("insertion du noeud 2 de degré 3 \n");
	insertkey(cd,m,2);
	printMinheap(m);
	
	printf("insertion du noeud 3 de degré 0 \n");
	insertkey(cd,m,3);
	printMinheap(m);
	free(cd);
	free_minheap(m);
	
}


int main(int argc,char** argv){

	printf("main Started...\n");
	testminHeap();
	/*
	adjarray* g;
	time_t t1,t2;
	int d;
	*/

	//t1=time(NULL);
	/*
	printf("Reading adjArray from file %s\n",argv[1]);
	g=readadjarray(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency array\n");

	mkadjarray(g);
	*/
	
	//insert kcore decomposition
	//free_adjarray(g);

	//t2=time(NULL);
 
    //printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	printf("main successfully ended !\n");
	return 0;
}
