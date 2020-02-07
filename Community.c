#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed
#define BUFFSIZELINE 100
#define NBITER 3



/*
typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

typedef struct {
	unsigned long value;
	int cluster;
}node;

typedef struct {
	node *s;
	node *t;
} edgeNode;


typedef struct{
	node * nodes;
	int size;
	int capacity;
}cluster;


typedef struct {
	edge * edges;
	//cluster * clusters;
	int nbclusters;	
}graphC;


edge * createEdge(unsigned long s,unsigned long t){
	edge* e=(edge*)malloc(sizeof(edge));
	e->s=s;
	e->t=t;
}
void free_graphC(graphC *g){
	if(g){
		free(g->edges);
		free(g);
	}

}


node * createNode(int value, int cluster){
	node* n=(node*)malloc(sizeof(node));
	n->value=value;
	n->cluster=cluster;
	return n;
}
*/
/*
edge * createEdgeNode(node* s, node* t){
	edge* e=(edge*)malloc(sizeof(edge));
	e->s=s;
	e->t=t;
	return e;
}
*/
/*
cluster *createCluster(int maxsize){
	cluster * c= (cluster*) malloc (sizeof(cluster));
	c->nodes=(node*)malloc(sizeof(node));
	c->size=0;
	c->capacity=maxsize;
	return c;
}
*/

/*
double rand_double() {
   return rand()/(double)RAND_MAX;
}

graphC * genGraphC(int nbnodes,int nbclusters,int sizecluster,double p,double q){
	int sc[nbclusters];
	graphC * g=(graphC*)malloc(sizeof(graphC));
	g->edges= (edge*)malloc(sizeof(edge));
	g->nbclusters=nbclusters;
	srand(time(NULL));   
     
	unsigned long i;
	int r;
	double rdouble;
	node * nodes= (node*)malloc(nbnodes*sizeof(node));

	for(i=0;i<nbnodes;i++){
			r=rand()%nbclusters;
			while(sc[r]==sizecluster){
				r=(r+1)%nbclusters;
			}
			nodes[i]=createNode(i,r);
	}
	

	srand((long)time(NULL));
	int u,v;
	for(u=0;u<nbnodes;u++){
		for(v=1;v<nbnodes;v++){
			rdouble= rand_double();
		
			if(nodes[u]->value!=nodes[v]->value)){
			
				if((nodes[u]->cluster==nodes[v]->cluster){
					if(rdouble<p) g->edges=createEdge(nodes[u]->value,nodes[v]->value);
			
				}else{
					if(rdouble<q) g->edges=createEdge(nodes[u]->value,nodes[v]->value);
				}
			}
			
		}
	}
	free(nodes);
	return g;
}
*/
typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//adjarray
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjarray;

//compute the maximum of three unsigned long
unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

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
//freeing memory
void free_adjarray(adjarray*g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g);
}

void shuffle(unsigned long * nodes,int size){
	srand(time(NULL));
	int i,j;
	unsigned long tmp;
	for(i=size-1;i>1;i--){
		j=random()%(i+1);
		tmp= nodes[i];
		nodes[i]=nodes[j];
		nodes[j]=tmp;
	}
}
unsigned long getHighestFreqLabel(adjarray* g, unsigned long node,unsigned long *labels){
	int i;
	unsigned long v;
	//frequency[i]= nombre d'occurence du label i 	
	unsigned long frequency[g->n];
	unsigned long max=0;
	unsigned long labelplusfrequent=0;
	//dénombrement du nombre d'occurence des labels de chacun de ses voisins
	for(i=(g->cd)[node];i<(g->cd)[node+1];i++){
		v=(g->adj)[i];
		frequency[labels[v]]++;
	}
	//récuperer le label le plus fréquent dans ses voisins
	for(i=(g->cd)[node];i<(g->cd)[node+1];i++){
		v=(g->adj)[i];
		if(frequency[labels[v]]>max) 
		{
			max=frequency[labels[v]];
			labelplusfrequent=labels[v];
		}
	}
	return labelplusfrequent;
}
int existsNotHighest(adjarray* g,unsigned long *labels,unsigned long * nodes){
	int i;
	for(i=0;i<g->n;i++){
		if(labels[nodes[i]]!=getHighestFreqLabel(g,nodes[i],labels))
			return 1;
	}
	return 0;
}
void LabelPropagation(adjarray*g){
	unsigned long labels[g->n];
	unsigned long nodes[g->n];
	int i;
	for(i=0;i<(g->n);i++){
		nodes[i]=(g->adj)[g->cd[i]];
		//step1
		labels[i]=i;
	}
	
	while(existsNotHighest(g,labels,nodes)){
		//step2
		shuffle(nodes,g->n);
		//step3
		for(i=0;i<(g->n);i++){
			labels[nodes[i]]=getHighestFreqLabel(g,nodes[i],labels);
		}
	
	}
	
}

int main(int argc,char** argv){
	adjarray* g;
	time_t t1,t2;
	
	t1=time(NULL);
	printf("Start main...");
	printf("Reading adjArray from file %s\n",argv[1]);
	g=readadjarray(argv[1]);
	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);
	
	printf("Building the adjacency array\n");
	mkadjarray(g);
	
	//unsigned long test[5]={1,2,3,4,5};
	//shuffle(test,5);
	//printf("[%d,%d,%d,%d,%d]\n",test[0],test[1],test[2],test[3],test[4],test[5]);
	//graphC* g=genGraphC(400,4,100,0.65,0.2);
	free_adjarray(g);
	t2=time(NULL);
	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	return 0;
}
