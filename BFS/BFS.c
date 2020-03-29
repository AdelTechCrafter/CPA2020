/*
Adel EL AMRAOUI
March 2020

to compile: gcc -o bfs BFS.c adjarray.c
to execute:
	-in main  choose the test (Bfs, diameter or NodesinlargestWC )
	-then ./bfs edgelist.txt
All the tests are working currently with email, amazon, and liveJournal graph files.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>//to estimate the runing time
#include<limits.h>
#include "adjarray.h"
#define NBITER 3


typedef struct {
	unsigned long * t;
	unsigned long * debut;
	unsigned long * fin;
	unsigned long size;
} fifo;

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
//classic BFS
void BFS(adjarray*g,unsigned long s){
	printf("Start of BFS...\n");
	fifo* f= makefifo(g);
	unsigned long *marked= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	add(f,s);
	marked[s]=1;
	unsigned long u;
	unsigned long d;
	unsigned long i;
	unsigned long v;
	int nbnodes=0;
	while(f->size >0){
		u=pop(f);
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
	free_fifo(f);
	printf("nombre de noeuds traverses: %i \n",nbnodes);
	printf("End of BFS !\n ");
}

//BFS version used to get the number of nodes in the largest connected component.
int BFSlargestWCC(adjarray*g,unsigned long s,unsigned long* output){
	printf("Start of BFSlargestWCC at node %lu...\n",s);
	fifo* f= makefifo(g);
	unsigned long *marked= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	add(f,s);
	marked[s]=1;
	//printf(" initial : f->size = %lu \n",f->size);
	unsigned long u;
	unsigned long d;
	unsigned long i;
	unsigned long v;
	int nbnodes=0;
	while(f->size >0){
		u=pop(f);
		*output=u;
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
	printf("End of BFSlargestWCC !\n");
	return nbnodes;
}
	//BFS version used to compute the diameter of a graph
	void BFS_diameter(adjarray*g,unsigned long s,unsigned long* output,int* d){
		printf("Start of BFS_diameter at node %lu...\n",s);
		fifo* f= makefifo(g);
		unsigned long u,i,v,j;
		add(f,s);
		for(j=0;j<g->n;j++) d[j]=-1;
		d[s]=0;
		while(f->size >0){
			u=pop(f);
			*output=u;
			for(i=(g->cd)[u];i<(g->cd)[u+1];i++){
				v=(g->adj)[i];
				if(d[v]==-1){
					add(f,v);
					d[v]= d[u]+1;
				}
			}
		}
		free_fifo(f);
		printf("End of BFS_diameter !\n");
}

int maxdist(int a, int b){
	return (a>b)?a:b;
}
//Diameter of a graph (longest shortest path)
int diameter(adjarray*g){
	//int i=0;
	unsigned long  max=0;
	unsigned long u,v;
	int dist1,dist2;
	int* d=(int*) calloc(g->n,sizeof(int));
	BFS_diameter(g,g->edges[0].s,&u,d);
	dist1=d[u];
  BFS_diameter(g,u,&v,d);
	dist2=d[v];
	free(d);
	printf("dist1=%d\n",dist1);
	printf("dist2=%d\n",dist2);
	return maxdist(dist1,dist2);
}
//number of nodes in the largest connected component
int NodesinlargestWCC(adjarray*g,int iterations){
	int i=0;
	unsigned long  max=0;
	unsigned long output;
	unsigned long lastnode;
	unsigned long  nbnodes;
	for(i=0;i<iterations;i++){
			if(i==0){
				nbnodes=BFSlargestWCC(g,g->edges[0].s,&output);
				max=nbnodes;
				lastnode=output;
				//if(nbnodes>max) max=nbnodes;
			}
			else{
				nbnodes=BFSlargestWCC(g,lastnode,&output);
				if(nbnodes>max) max=nbnodes;
				lastnode=output;
			}
	}
	return max;
}

void testBFS(	adjarray* g){
	BFS(g,g->edges[0].s);
}
void testNodesinlargestWCC(adjarray* g){
	printf("number of nodes in the largest connected component = %d\n",NodesinlargestWCC(g,NBITER));
}
void testDiameter(adjarray* g){
	printf("diameter = %d\n",diameter(g));
}
int main(int argc,char** argv){
	adjarray* g;
	time_t t1,t2;
	t1=time(NULL);

	printf("Reading adjArray from file %s\n",argv[1]);
	g=read_adjarray(argv[1]);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency array\n");
	mkadjarray(g);
	printf("adjacency array is ready\n");

	//testBFS(g);
	//testNodesinlargestWCC(g);
	testDiameter(g);
	free_adjarray(g);
	t2=time(NULL);
	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	return 0;
}
