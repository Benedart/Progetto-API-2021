#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 4000
#define MAXK 300000
#define MAXREAD 10000

#pragma GCC optimize("Ofast")

typedef struct s_heapNode{
	int index;
	long long dist;
}heapNode;

typedef struct s_heap{
	int size;
	heapNode* A;
}heap;

heap h[2];
unsigned long* distances;

int isHeap(heap*);
void printHeap(heap*);
void swap(heapNode*, heapNode*);
int getParent(int);
int check(heapNode, heapNode);
void minHeapify(heap*, int);
void heapInsert(heap*, heapNode);
void updateHeap(heap*, heapNode);
heap* createHeap(heap*, int);
heapNode cancellaMin(heap*);
long long dijsktra(unsigned long* g, int, int);

int main(int argc, char* argv[]){
	int n, k;
	char c;
	unsigned long* m;
	heap* ranks;
	int curr = 0;
	long long score;
	char* buffer;

	#ifdef BENEDAR
		freopen("gen_tests/private/input.1", "r", stdin);
//		freopen("/dev/null", "w", stdout);
	#endif

	if(scanf("%d", &n));
	if(scanf("%d", &k));

	ranks = createHeap(&(h[0]), k+1);
	distances = malloc(n * sizeof(unsigned long));
	m = malloc(n * n * sizeof(unsigned long));
	buffer = malloc(MAXREAD * sizeof(char));
	
	while((c = getchar_unlocked()) != EOF){
		if(c == 'A'){
			while((c = getchar_unlocked()) != '\n');
			for(int i=0; i<n; i++){
				if(fgets(buffer, MAXREAD, stdin));

//				printf("%d: %s", i, buffer);

				int count = 0;
				unsigned long num = 0;
				for(int j=0; buffer[j] != '\n'; j++){

					if(buffer[j] == ','){
						*(m + n*i + count) = num;
						num = 0;
						count++;
					}else if(buffer[j] >= '0' && buffer[j] <= '9') num = num*10 + (int)(buffer[j]-'0');
				}
				*(m + n*i + count) = num;
			}

			score = dijsktra(m, 0, n);

			heapNode newNode;
			newNode.index = -curr;
			newNode.dist = -score;
			heapInsert(ranks, newNode);
			if(curr >= k) cancellaMin(ranks);

			curr++;
		}else if(c == 'T'){
			int lim;

			if(curr < k) lim = curr;
			else lim = k;

			for(int i=0; i<lim-1; i++){
				printf("%d ", -ranks->A[i].index);
			}
			if(lim != 0) printf("%d", -ranks->A[lim-1].index);
			printf("\n");
		}
	}

	free(m);
	free(buffer);
	free(h[0].A);
	free(h[1].A);
	free(distances);

	return 0;
}

void swap(heapNode* a, heapNode* b){
	heapNode temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int getParent(int key){
	if(key == 0)
		return -1;
	return (int)((key-1)/2);
}

int check(heapNode a, heapNode b){
	if(a.dist == b.dist)
		return a.index < b.index;
	return a.dist < b.dist;
}

void minHeapify(heap* h, int n){
	int posmin;
	int l = 2*n + 1;	// figlio sinistro
	int r = 2*n + 2;	// figlio destro

	if(l < h->size && check(h->A[l], h->A[n]))
		posmin = l;
	else posmin = n;

	if(r < h->size && check(h->A[r], h->A[posmin]))
		posmin = r;

	if(posmin != n){
		swap(&(h->A[n]), &(h->A[posmin]));
		minHeapify(h, posmin);
	}
}

void heapInsert(heap* h, heapNode node){
	int i;

//	printf("%d\n", h->size);
	h->A[h->size] = node;
	h->size++;
	i = h->size - 1;

	while(i > 0 && check(h->A[i], h->A[getParent(i)])){
		swap(&(h->A[getParent(i)]), &(h->A[i]));
		i = getParent(i);
	}

//	printHeap(h);
/*	for(int i=1; i<h->size; i++)
		printf("%d %d\n", i, check(h->A[getParent(i)], h->A[i]));
*/
}

void updateHeap(heap* h, heapNode node){
	h->A[node.index].dist = node.dist;
	minHeapify(h, node.index);
}

heap* createHeap(heap* h, int dim){
	h->size = 0;

	if(h->A == NULL)
		h->A = calloc(dim, sizeof(heapNode));
	else memset(h->A, 0, dim*sizeof(heapNode));
	
	return h;
}

heapNode cancellaMin(heap* h){
	heapNode min;
	if(h->size == 0){
		min.index = -1;
		min.dist = 0;
		return min;
	}

	min = h->A[0];
	h->A[0] = h->A[h->size - 1];
	h->size--;
	minHeapify(h, 0);

	return min;
}

long long dijsktra(unsigned long* g, int s, int n){
	heap* queue = createHeap(&(h[1]), n*n);
	unsigned long ndis;
	long long sum = 0;
	heapNode u;

	distances[s] = 0;

	for(int i=0; i<n; i++){
		if(i != s){
			distances[i] = 1e12;
		}

		heapNode newNode;
		newNode.index = i;
		newNode.dist = distances[i];
		heapInsert(queue, newNode);
	}
	while(queue->size != 0){
		u = cancellaMin(queue);

	//	printf("%d %lld\n", u.index, u.dist);

		for(int i=0; i<n; i++){
			if(*(g + n*u.index + i) != 0){		// se stiamo considerando uno dei nodi adiacenti a u
				ndis = distances[u.index] + *(g + n*u.index + i);

				if(distances[i] > ndis){
					distances[i] = ndis;

					heapNode node;
					node.index = i;
					node.dist = ndis;
					heapInsert(queue, node);
				}
			}
		}
	}

	for(int i=0; i<n; i++){
	//	printf("%d ", distances[i]);
		if(distances[i] != 1e12)
			sum += (long long) distances[i];
	}
	//printf("\n");

	return sum;
}






int isHeap(heap* h){
	for(int i=0; i<h->size; i++)
		if(h->A[i].dist < h->A[getParent(i)].dist)
			return 0;

	return 1;
}

void printHeap(heap* h){
	for(int i=0; i<h->size; i++)
		printf("%d %lld\n", h->A[i].index, h->A[i].dist);
	printf("\n");
}