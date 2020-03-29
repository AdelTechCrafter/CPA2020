/*
Maximilien Danisch
September 2017
http://bit.ly/danisch
maximilien.danisch@gmail.com

Info:
Feel free to use these lines as you wish. This program loads a graph in main memory as an adjacency list.

To compile:
"gcc adjarray.c -O9 -o adjarray.exe".

To execute:
"./adjarray.exe edgelist.txt".
"edgelist.txt" should contain the graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
The prograph will load the graph in main memory and then terminate.

Performance:
Up to 200 million edges on my laptop with 8G of RAM: takes more or less 4G of RAM and 30 seconds (I have an SSD hardrive) for 100M edges.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the running time
#include "adjarray.h"

//compute the maximum of three unsigned long
static inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}
//reading the adjarray from file (skip the first 5 lines)
adjarray* read_adjarray(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");
	adjarray *g=malloc(sizeof(adjarray));
	char buffer[BUFFSIZELINE];
	g->n=0;
	g->e=0;
	g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges

	fgets(buffer, BUFFSIZELINE, file);
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

//building the adjacency array
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
}


//freeing memory
void free_adjarray(adjarray *g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g);
}
