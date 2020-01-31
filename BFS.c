
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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>//to estimate the runing time

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed
#define BUFFSIZELINE 100

typedef struct {
	unsigned long * t;
	unsigned long * debut;
	unsigned long * fin;
	unsigned long size;
} fifo;

typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjarray;

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the adjarray from file
adjarray* readadjarray(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");
	//ajout
	char buffer[BUFFSIZELINE]; 

	adjarray*g=malloc(sizeof(adjarray));
	g->n=0;
	g->e=0;
	g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges
	
	//ajout
	fgets(buffer, BUFFSIZELINE, file);
	fgets(buffer, BUFFSIZELINE, file);
	fgets(buffer, BUFFSIZELINE, file);
	fgets(buffer, BUFFSIZELINE, file);
	
	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (++(g->e)==e1) {//increase allocated RAM if needed
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}


//building the adjacency matrix
void mkadjarray(adjarray* g){
	unsigned long i,u,v;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(2*g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(d);
	//free(g->edges);
}

fifo*  makefifo(adjarray*g){
	fifo * f= (fifo *) malloc(sizeof(fifo));
	f->t=(unsigned long*) malloc((g->n)*sizeof(unsigned long));
	f->size=0;
	f->debut=f->t;
	f->fin= f->t;
	return f;
}
void add(fifo *f, unsigned long v){
	*(f->fin)=v;
	(f->fin)++;
	(f->size)++;
}
void free_fifo(fifo*f){
	free(f->t);
	free(f);
}

//freeing memory
void free_adjarray(adjarray*g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g);
}



unsigned long pop (fifo*f ){
	if((f->size)<=0){
		printf("Error : pop on empty FIFO \n ");
		exit(0);
	}
	
	unsigned long res= *(f->debut);
	(f->debut)++;
	(f->size)--;
	return res;
}


void BFS(adjarray*g,unsigned long s){
	printf("Start of BFS...\n ");	
	fifo* f= makefifo(g);
	unsigned long *marked= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	add(f,s);
	marked[s]=1;
	printf(" initial : f->size = %lu \n",f->size);
	
	unsigned long u;
	unsigned long d;
	unsigned long i;
	unsigned long v;
	int nbnodes=0;
	while(f->size >0){
		u=pop(f);
		//printf("f->size = %lu \n",f->size);
		printf("u= %lu \n",u);
		nbnodes++;
		for(i=(g->cd)[u];i<(g->cd)[u+1];i++){
			v=(g->adj)[i];
			if(marked[v]!=1){
				add(f,v);
				marked[v]=1;
			}
		}
	}
	
	free(marked);
	printf("free (marked) passed !\n");
	free_fifo(f);
	printf("free_fifo passed !\n");
	
	printf("nombre de noeuds traversés: %i \n",nbnodes);
	printf("End of BFS !\n ");			
	
}
/*
unsigned long diameter(adjarray*g,int iterations){
	int i;
	srand(time(NULL)); 
	int r = rand();      
	for(i=0;i<iterations;i++){
		
		BFS(g,)
	}
}
*/

int main(int argc,char** argv){
	adjarray* g;
	time_t t1,t2;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readadjarray(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency array\n");
	
	mkadjarray(g);
	BFS(g,0);
	free_adjarray(g);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	
	
	return 0;
}




