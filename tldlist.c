// @author Shayam N. Bhudia
// @guid 2394138b
// Systems Programming - Coursework 1

#include "date.h"
#include "tldlist.h"
#include <stdio.h>
#include <string.h>

// acts as the ADT node class (java implementation)
// the ADT node itself
struct tldnode {
	Date date;
	char country_code[];
	int count;
	TLDNode *r_child, *l_child;
};

struct tldlist {
	Date begin;
	Date end;
	int elems_n;
	int country_codes_n;
	TLDNode rootNode;
	TLDNode nodeList[];
};

struct tlditerator {
	TLDNode nodes[];
	long next_elem;
	long length;
};


/*** util functions for BST handling ***/

TLDNode *create_node(Date d, char *country_code) {
	TLDNode *node = (TLDNode *)malloc(sizeof(TLDNode));
	node->date = d;
	node->country_code = country_code;
	node->left_child = NULL;
	node->right_child = NULL;
	node->count = 1;

	return node;
}

TLDNode *addNode(TLDNode *root, Date *d, char *country_code, TLDList *tld) {
    //searching for the place to insert the new node (recursively)
    if (root == NULL) {
    	tld->rootNode = root;
		return create_node(d, country_code);

	} else if (root->country_code == country_code) {
		root->count = root->count + 1;
		return root;
	} else if (strncmp(country_code, root->country_code, 4) > 0) {
		root->right_child = addNode(root->right_child, d, country_code, tld);

	} else {
		root->left_child = addNode(root->left_child, d, country_code, tld);
	}

	return root;
}

// extract the country code from the hostname
char *get_country_code(char *hostname) {
	char country_code[] = "";
	int n = strlen(hostname);
	slice_str(hostname, country_code, n - 3, n);
	if (strcmp(country_code[0], ".") == 0) {
		char country_code_short[] = "";
		slice_str(country_code, country_code_short, 1, 2); // remove the "."
		return country_code_short;
	}
	return country_code;
}

// take substring of a given string
void slice_str(const char * str, char * buffer, size_t start, size_t end) {
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

TLDNode *search_BST(TLDNode *root, country_code) {
	if (root == NULL || root->country_code==country_code) { // if root->country_code == country_code then the element is found
		return root;
	}
    else if (strncmp(country_code, root->country_code, 4) > 0) { // search the right subtree
		return search_BST(root->right_child, country_code);
    }
    else { // search the left subtree
		return search_BST(root->left_child,country_code);
	}
}

int look_for_node(TLDNode *root, country_code) {
	TLDNode foundNode = search_BST(root, country_code);
	if (foundNode) {
		return 1
	}
	return -1;
}

/*** Core program (provided functions) ***/

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end) {
	TLDList *tld = (TLDList *)malloc(sizeof(TLDList));
	tld->begin = begin;
	tld->end = end;
	tld->elems_n = 0;
	tld->nodeList = {};
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld) {
	free(tld);
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d) {
	// check for valid date
	int validBeginDate = date_compare(tld->begin, d);  // valid if -1
	int validEndDate = date_compare(tld->end, d);  // valid if 1
	int atBegin = date_compare(tld->begin, d);  // valid if 0
	int atEnd = date_compare(tld->end, d);  // valid if 0

	if (validBeginDate == -1 || validEndDate == 1 || atBegin ==0 || atEnd ==0) {
		char country_code = get_country_code(hostname);
		TLDNode node = addNode(tld, country_code, d, tld);
		tld->elems_n = tld->elems_n + 1;
		int unique = look_for_node(tld->rootNode, node->country_code);
		if (unique < 0) { // the node is unique
			tld->country_codes_n = tld->country_codes_n + 1;
		}
		tld->nodeList[tld->elems_n] = node;
		return 1;
	} else {
		return 0;
	}
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld) {
	return (long)tld->elems_n;
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld) {
	TLDIterator *iter = (TLDIterator *)malloc(sizeof(TLDIterator));

	if (iter) { // if iter is not NULL (if NULL returns NULL)
		iter->nodes = malloc(sizeof(TLDNode*)*tld->country_codes_n);
		// BST_to_array(iter, tld->rootNode);
		iter->nodes = tld->nodeList;
		iter->next_elem = 0;
		iter->size = tld->country_codes_n;
	}
	return iter;
}

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter) {
	if (iter->size == iter->next_elem) {
		return NULL;
	}
	iter->next_elem = iter->next_elem + 1;
	return iter->nodes[iter->next_elem - 1];
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter) {
	free(iter);
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node) {
	return node->hostname;
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node) {
	return (long)node->count;
}
