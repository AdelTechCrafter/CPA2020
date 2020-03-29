/*Adel EL AMRAOUI
March 2020

to compile : gcc -o PageRankExo2 PageRankExo2.c adjarray.c
to execute : ./PageRankExo2.exe edgelist.txt
it generates 6 files, each file correponding to one test of the exercise2
approx execution time: 1min30 seconds
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include "adjarray.h"
#define T 15
//to compile : gcc -o PageRankExo2 PageRankExo2.c adjarray.c
//to execute : ./PageRankExo2.exe edgelist.txt
//it generates 6 files, each file correponding to one test of the exercise2
//approx execution time: 1min30 seconds
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
//PageRank using power-iteration method
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
void generateEx2files_1_2(const char * name1,const char * name2,adjarray* g,double* p){
  printf("generating files %s and %s...\n",name1,name2);
  unsigned long* indegree=calloc(g->n, sizeof(unsigned long));
  unsigned long* outdegree=calloc(g->n, sizeof(unsigned long));
  FILE* f1=fopen(name1, "w" );
  FILE* f2=fopen(name2, "w" );
  int i;
  for (i=0; i<g->e; i++) {
      indegree[g->edges[i].t]++;
      outdegree[g->edges[i].s]++;
    }
  for(i=0; i<g->n; i++) {
        if(p[i] > 0){
           if(indegree[i]>0) fprintf(f1,"%e %lu\n", p[i], indegree[i]);
           if(outdegree[i]>0) fprintf(f2,"%e %lu\n", p[i], outdegree[i]);
        }
  }
  fclose(f1);
  fclose(f2);
  free(indegree);
  free(outdegree);
  printf("files %s and %s generated\n",name1,name2);
}
void generateEx2file_3_4_5_6(const char * name,adjarray* g,double* p1,double* p2){
  printf("generating file %s...\n",name);
  int i;
  FILE* f=fopen(name, "w" );
  for (i=0; i<g->n; i++) {
    if((p1[i] > 0) && (p2[i] > 0)){
      fprintf(f,"%e %e\n",p1[i],p2[i]);
    }
  }
  fclose(f);
  printf("file %s generated\n",name);
}
//main exo2
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
  double* p1 = PageRank(g,0.15,T);
  double* p2 = PageRank(g,0.1,T);
  double* p3 = PageRank(g,0.2,T);
  double* p4 = PageRank(g,0.5,T);
  double* p5 = PageRank(g,0.9,T);
  generateEx2files_1_2("generatedFiles/correlation1.txt","generatedFiles/correlation2.txt",g,p1);
  generateEx2file_3_4_5_6("generatedFiles/correlation3.txt",g,p1,p2);
  generateEx2file_3_4_5_6("generatedFiles/correlation4.txt",g,p1,p3);
  generateEx2file_3_4_5_6("generatedFiles/correlation5.txt",g,p1,p4);
  generateEx2file_3_4_5_6("generatedFiles/correlation6.txt",g,p1,p5);
  free(p1);
  free(p2);
  free(p3);
  free(p4);
  free(p5);
 	free_adjarray(g);
 	t2=time(NULL);
 	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
 	printf("_______________________________________________________________________\n\n");
	return 0;
}
