#ifndef ADJARRAY_H_
#define ADJARRAY_H_

#define NLINKS 100000000//maximum number of edges for memory allocation, will increase if needed
#define BUFFSIZELINE 300
typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//adjacency list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjarray;

static inline unsigned long max3(unsigned long a, unsigned long b, unsigned long c);
adjarray* read_adjarray(char* input);
void mkadjarray(adjarray* g);
void free_adjarray(adjarray *g);

#endif /* ADJARRAY_H_ */
