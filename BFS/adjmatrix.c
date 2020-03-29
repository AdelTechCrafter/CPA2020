/*
Maximilien Danisch
September 2017
http://bit.ly/danisch
maximilien.danisch@gmail.com
Info:
Feel free to use these lines as you wish. This program loads an unweighetd graph in main memory as an adjacency matrix.
To compile:
"gcc adjmatrix.c -O9 -o adjmatrix".
To execute:
"./adjmatrix edgelist.txt".
"edgelist.txt" should contain the graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
The prograph will load the graph in main memory and then terminate.
Note:
If the graph is directed (and weighted) with selfloops and you want to make it undirected unweighted without selfloops, use the following linux command line.
awk '{if ($1<$2) print $1" "$2;else if ($2<$1) print $2" "$1}' net.txt | sort -n -k1,2 -u > net2.txt
Performence:
Up to 200.000 nodes on my laptop with 8G of RAM.
Takes more or less 4G of RAM and 10 seconds (I have an SSD hardrive) for 100.000 nodes.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>//to estimate the runing time

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed
#define BUFFSIZELINE 200
typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	bool *mat;//adjacency matrix
} adjmatrix;

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

//reading the edgelist from file
adjmatrix* readedgelist(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");
  char buffer[BUFFSIZELINE];
	adjmatrix *g=malloc(sizeof(adjmatrix));
	g->n=0;
	g->e=0;
	g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges
  //skip the first 4 lines
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
void mkmatrix(adjmatrix* g){
	unsigned long i,u,v;
	g->mat=calloc(g->n*g->n,sizeof(bool));
	for (i=0;i<g->e;i++){
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->mat[u+g->n*v]=1;
		g->mat[v+g->n*u]=1;
	}
}


void free_adjmatrix(adjmatrix *g){
	free(g->edges);
	free(g->mat);
	free(g);
}

int main(int argc,char** argv){
	adjmatrix* g;
	time_t t1,t2;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency matrix\n");
	mkmatrix(g);

	free_adjmatrix(g);

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	return 0;
}
