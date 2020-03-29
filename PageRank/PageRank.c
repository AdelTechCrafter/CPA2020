/*
Adel EL AMRAOUI
March 2020

to compile : gcc -o PageRank PageRank.c adjarray.c
to execute : ./PageRank edgelist.txt
it generates 1 file: generatedFiles/results.txt
to generate sortedbyPageRank.txt: sort -g -k2 < results.txt > sortedbyPageRank.txt
to generate Top5PageRank: head -5 sortedbyPageRank.txt > Top5PageRank.txt
to generate Bot5PageRank: tail -5 sortedbyPageRank.txt > Bot5PageRank.txt
 to find names of pages : grep "^idpage[[:space:]]" "alr21--pageNum2Name--enwiki-20071018.txt"*/
 //approx execution time: 40 seconds
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include "adjarray.h"
#define ALPHA 0.15
#define T 15

bool almosteq(double a, double b)
{
    return (fabs(a-b) < 0.00001);
}

void PRODMATVECT(adjarray* g,unsigned long* outdegree, double *p) {
	int i;
	double *tmp = calloc(g->n, sizeof(double));
  unsigned long s;
  unsigned long t;
  for(i=0; i < g->e; i++) {
        s = g->edges[i].s;
        t = g->edges[i].t;
        if (outdegree[s] > 0)
          tmp[t] += p[s]/((double)(outdegree[s]));
    }
	for(i = 0; i < g->n; i++)
    p[i] = tmp[i];
	free(tmp);
}
void NORMALIZE2(double *p, int nbnodes) {
    double somme = 0;
    int i;
    for(i=0; i<nbnodes; i++)
      somme += p[i];
    for(i=0; i<nbnodes; i++)
      p[i] += (1.0-somme)/(double)nbnodes;
}
int checkConvergence(double *p1, double *p2, int nbnodes,int iteration){
  int i;
  int convergence=1;
  for(i=0;i<nbnodes;i++){
    if(!almosteq(p1[i],p2[i])) {
      convergence=0;
      break;
    }
  }
  return convergence;
}
//PageRank using power-iteration method : stopping when reaching convergence or the number of iterations t
double* PageRank(adjarray* g, double alpha, int t) {
    printf("Starting PageRank with alpha=%2f and t=%d\n",alpha,t);
    int i,j,k;
    double* p = calloc(g->n, sizeof(double));
    double* tmpconvergence = calloc(g->n,sizeof(double));
	  unsigned long* outdegree = calloc(g->n, sizeof(unsigned long));
    int nbnodes=0;
    int checkconvergence;
    for (i=0; i<g->e; i++) {
        outdegree[g->edges[i].s]++;
    }
    for (i=0; i<g->n; i++) {
        if (outdegree[i] > 0) {
            nbnodes++;
        }
    }
    //first state init : same probability for each node
    double I= 1.0/(double)nbnodes;
    for (i=0; i<g->n; i++) {
        if (outdegree[i] > 0) {
            p[i] =I;
        }
    }
    //copy of p for convergence
    for (i=0; i<g->n; i++) {
      tmpconvergence[i]=p[i];
    }
    for(i=0; i<t; i++) {
        printf("iteration: %d\n",i+1);
        //P ← PRODMATVECT(T, P)
        PRODMATVECT(g,outdegree,p);
        //P ← (1 − α) × P + α × I
        for (j=0; j<g->n; j++) {
          if (outdegree[j] > 0) {
            p[j] = (1.0-alpha)*p[j]+alpha*I;
          }
		     }
        //P ← NORMALIZE2(P)
        NORMALIZE2(p, nbnodes);
        checkconvergence=checkConvergence(tmpconvergence,p,g->n,i);
        if(checkconvergence){
            printf("Convergence atteinte a l'iteration %d\n",i+1);
            break;
        }

        //copy of p for convergence
        for (k=0; k<g->n; k++) {
          tmpconvergence[k]=p[k];
        }

    }
    free(tmpconvergence);
    free(outdegree);
    printf("PageRank with alpha=%2f and t=%d successfully ended !\n",alpha,t);
    return p;
}
//main ex1
int main(int argc,char** argv){
  adjarray* g;
 	time_t t1,t2;
 	t1=time(NULL);

 	printf("Reading adjarray from file %s\n",argv[1]);
 	g=read_adjarray(argv[1]);

 	printf("Number of nodes: %lu\n",g->n);
 	printf("Number of edges: %lu\n",g->e);

 	printf("Building the adjacency array\n");
 	mkadjarray(g);
  double* p = PageRank(g, ALPHA, T);
  printf("generating file results.txt...\n");
	FILE* f= fopen( "generatedFiles/results.txt", "w" );
  int i;
	for(i=0; i<g->n; i++) {
        if(p[i] > 0) fprintf(f,"%d %e\n", i, p[i]);
  }
  fclose(f);
  free(p);
 	free_adjarray(g);
 	t2=time(NULL);
 	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
 	printf("_______________________________________________________________________\n\n");
	return 0;
}
