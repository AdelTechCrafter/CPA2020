/*
Adel EL AMRAOUI
March 2020

to compile:gcc -o labelpropagation LabelPropagation.c adjarray.c
to execute:./labelpropation edgelist.txt
note: to read email-Eu-core.txt  uncomment lines 40 to 44 in adjarray.c to skip the first 4 lines
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "adjarray.h"
#define NBITER 3

void shuffle(unsigned long * nodes,int size){
	//printf("Start of Shuffle\n");
	int i,j;
	unsigned long tmp;
	for(i=size-1;i>1;i--){
		j=rand()%(i+1);
		tmp= nodes[i];
		nodes[i]=nodes[j];
		nodes[j]=tmp;
	}
	//printf("End of Shuffle\n");
}
unsigned long getHighestFreqLabel(adjarray* g, unsigned long node,unsigned long *labels){
	//printf("Start of getHighestFreqLabel\n");
	int i;
	unsigned long v;
	//frequency[i]= nombre d'occurence du label i
	unsigned long*  frequency= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	unsigned long max=0;
	unsigned long labelplusfrequent=0;
	/*dénombrement du nombre d'occurence des labels de chacun de ses voisins
	+ update du label le plus frequent*/
	for(i=(g->cd)[node];i<(g->cd)[node+1];i++){
		v=(g->adj)[i];
		frequency[labels[v]]++;
		if (frequency[labels[v]]>max){
			max=frequency[labels[v]];
			labelplusfrequent=labels[v];
		}
		//printf("frequence[%lu]=%lu\n",labels[v],frequency[labels[v]]);
	}
	//printf("node %lu : label le plus frequent dans ses voisins:%lu\n",node,labelplusfrequent);
	return labelplusfrequent;
	//printf("End of getHighestFreqLabel\n");
}
int existsNotHighest(adjarray* g,unsigned long *labels,unsigned long * nodes){
	//printf("Start of existsNotHighest\n");
	int i;
	for(i=0;i<g->n;i++){
		if(labels[nodes[i]]!=getHighestFreqLabel(g,nodes[i],labels))
			return 1;
	}
	return 0;
	//printf("end of existsNotHighest\n");
}
void LabelPropagation(adjarray*g,	unsigned long* labels){
	printf("Start of labelpropagation\n");
	unsigned long*  nodes= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	unsigned long i;
	for(i=0;i<(g->n);i++){
		nodes[i]=(g->adj)[g->cd[i]];
		//step1
		labels[i]=i;
	}
	srand(time(NULL));
	while(existsNotHighest(g,labels,nodes)){
		//step2
		shuffle(nodes,g->n);
		//step3
		for(i=0;i<(g->n);i++){
			labels[nodes[i]]=getHighestFreqLabel(g,nodes[i],labels);
		}
	}
	free(nodes);
	printf("end of labelpropagation\n");
}
void generatelabelpropagation1(adjarray* g){
	unsigned long*  labels= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	LabelPropagation(g,labels);
	printf("generating file labelpropagationgraph1.txt...\n");
	FILE* f= fopen( "labelpropagationgraph1.txt", "w" );
	unsigned long i;
	for(i=0; i<g->n; i++) {
				fprintf(f,"%lu %lu\n", i,labels[i]);
	}
	fclose(f);
	free(labels);
}

void generatelabelpropagation2(	adjarray* g){
	unsigned long*  labels= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	LabelPropagation(g,labels);
	printf("generating file labelpropagationgraph2.txt...\n");
	FILE* f= fopen( "labelpropagationgraph2.txt", "w" );
  unsigned long i;
	for(i=0; i<g->n; i++) {
        fprintf(f,"%lu %lu\n", i,labels[i]);
  }
	fclose(f);
	free(labels);
}

void generatelabelpropagation3(	adjarray* g){
	unsigned long*  labels= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	LabelPropagation(g,labels);
	printf("generating file labelpropagationgraph3.txt...\n");
	FILE* f= fopen( "labelpropagationgraph3.txt", "w" );
  unsigned long i;
	for(i=0; i<g->n; i++) {
        fprintf(f,"%lu %lu\n", i,labels[i]);
  }
	fclose(f);
	free(labels);
}

void generatelabelpropagationemail(	adjarray* g){
	unsigned long*  labels= (unsigned long*) calloc(g->n,sizeof(unsigned long));
	LabelPropagation(g,labels);
	printf("generating file labelpropagationemail.txt...\n");
	FILE* f= fopen( "labelpropagationemail.txt", "w" );
  unsigned long i;
	for(i=0; i<g->n; i++) {
        fprintf(f,"%lu %lu\n", i,labels[i]);
  }
	fclose(f);
	free(labels);
}
int main(int argc,char** argv){
	adjarray* g;
	time_t t1,t2;

	t1=time(NULL);
	printf("Start main...");
	printf("Reading adjArray from file %s\n",argv[1]);
	g=read_adjarray(argv[1]);
	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency array\n");
	mkadjarray(g);
	//generatelabelpropagation1(g);
	//generatelabelpropagation2(g);
	generatelabelpropagation3(g);
	//generatelabelpropagationemail(g);
	free_adjarray(g);
	t2=time(NULL);
	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	return 0;
}
