#include <stdio.h>
#include <stdlib.h>

// 1 2 1
// 1 3 5
// | 4 | 4 | 4 | 8 | 8 | 8 | 8 |

#if CHECK_ASSERTIONS
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

struct Allocator {
	char *begin;
	char *end;
	char *capacity;
};

void Allocator_init(struct Allocator *self, int capacity)
{
	Assert(capacity > 0);
	self->begin = (char*) malloc(capacity);
	self->end = self->begin;
	self->capacity = self->begin + capacity;
}

void *Allocator_alloc(struct Allocator *self, int size) {
	Assert(self->end + size <= self->capacity);
	void *result = (void*) self->end;
	self->end += size;
	return result;
}

struct Node;

struct Neighbor {
	struct Node *node;
	int         weight;
};

struct Node {
	int             id;
	int             in_degree;
	int             out_degree;
	struct Neighbor *in_neighbors;
	struct Neighbor *out_neighbors;
};

void Node_init(struct Node *self, int id)
{
	self->id = id;
	self->in_degree = 0;
	self->out_degree = 0;
	self->in_neighbors = 0;
	self->out_neighbors = 0;
}


int main()
{
	struct Allocator allocator;
	Allocator_init(&allocator,Megabytes(1));

	int input[] = {
		1, 2, 3,
		1, 3, 5,
		1, 5, 7,
		2, 4, 3,
		3, 4, 5,
		3, 5, 10,
		4, 7, 5,
		5, 6, 17,
		6, 7, 17
	};
	int num_arcs = sizeof(input) / (3 * sizeof(int));

	/* find number of nodes*/
	int max_node=0;
	for (int i=0;i<num_arcs;++i) {
		int a = input[i*3];
		int b = input[i*3+1];
		max_node = max_node > a ? max_node : a;
		max_node = max_node > b ? max_node : b;
	}

	/* allocate space for nodes and initialize empty nodes*/
	struct Node *nodes = (struct Node*)
		Allocator_alloc(&allocator,
				sizeof(struct Node) * (max_node + 1));
	for (int i=0;i<=max_node;++i) {
		Node_init(nodes + i, i);
	}

	/* find in and out degree of nodes*/
	for (int i=0;i<num_arcs;++i) {
		int a = input[i*3];
		int b = input[i*3+1];
		struct Node* a_node = nodes + a;
		struct Node* b_node = nodes + b;
		++a_node->out_degree;
		++b_node->in_degree;
	}

	/*
	 * reserve exact space for adjacent lists
	 * of nodes and weights
	 */
	for (int i=0;i<=max_node;++i) {
		struct Node *node = nodes + i;

		if (node->in_degree > 0) {
			node->in_neighbors = (struct Neighbor*)
				Allocator_alloc(&allocator,
						 sizeof(struct Neighbor) * node->in_degree);
			node->in_degree = 0;
		}

		if (node->out_degree > 0) {
			node->out_neighbors = (struct Neighbor*)
				Allocator_alloc(&allocator,
						sizeof(struct Neighbor) * node->out_degree);
			node->out_degree = 0;
		}
	}

	/* initialize neighbors and their weights*/
	for (int i=0;i<num_arcs;++i) {
		int a = input[i*3];
		int b = input[i*3+1];
		int w = input[i*3+2];

		struct Node* a_node = nodes + a;
		struct Node* b_node = nodes + b;

		struct Neighbor* a_neighbor = a_node->out_neighbors +
			a_node->out_degree;
		a_neighbor->node = b_node;
		a_neighbor->weight = w;
		++a_node->out_degree;

		struct Neighbor* b_neighbor = b_node->in_neighbors +
			b_node->in_degree;
		b_neighbor->node = a_node;
		b_neighbor->weight = w;
		++b_node->in_degree;
	}

}
