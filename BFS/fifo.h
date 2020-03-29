#ifndef FIFO_H_
#define FIFO_H_

typedef struct {
	unsigned long * t;
	unsigned long * debut;
	unsigned long * fin;
	unsigned long size;
} fifo;




static inline unsigned long max3(unsigned long long a, unsigned long long b, unsigned long long c);
adjarray* read_adjarray(char* input);
void mkadjarray(adjarray* g);
void free_adjarray(adjarray *g);

#endif /* ADJARRAY_H_ */
