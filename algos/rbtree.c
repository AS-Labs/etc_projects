#include <stdio.h>

struct rbtree_node {
	enum { red, black } color;
	void *item;
	struct rbtree_node *left,
		*right,
		*parent;
};
