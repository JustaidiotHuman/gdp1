// Implementation for unsorted single linked list of integers:
// tail-recursive version
//
// Demonstrate, that the compiler generates a tight loop for list_insert_end_aux
//
// make
// gdb -q  -ex "disassemble list_insert_end_aux" -ex "quit" bin/singleLinkedIntList_functional_tail_recursive_demo 
//
// NOTE:
// We use a very simple error handling method.
// If anything is wrong, we bail out with an exit()

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "singleLinkedIntList_type.h"

// Create a new node and return its pointer
node_t* list_create_node(int data) {
  node_t* newnode;
  // Create a new node for data
  if ((newnode = malloc(sizeof(node_t))) == NULL) { 
    fprintf(stderr,"list_create_node: Unable to create a new data node\n");
    // Here we bail out without any attempts for recovery
    // This is a very simplistic strategie
    exit(EXIT_FAILURE);
  } else {
    // Initialize node
    newnode -> data = data;
    newnode -> next = NULL;
    return  newnode;
  }
}

// Insert data at the end of the list

/* Original deep recursive version
node_t* list_insert_end(node_t* node, int data) {
  // Is the list empty
  if (node == NULL) {
    return list_create_node(data);
  } else {
    // Recursive call
    node -> next = list_insert_end(node->next, data);
    return node;
  }
}
*/

// Tail-recursive version
// Invariant: first != NULL && last != NULL
node_t* list_insert_end_aux(node_t* cur, int data, node_t* first, node_t* last) {
  if (cur == NULL){
    last -> next = list_create_node(data);
    return first;
  } else {
    return list_insert_end_aux(cur->next, data, first, cur);
  }
}

// A wrapper with the usual functional interface
node_t* list_insert_end(node_t* node, int data) {
  if (node == NULL) {
    return list_create_node(data);
  } else {
    return list_insert_end_aux(node->next, data, node, node);
  }
}

// Dump all nodes of a list inclusive information about addresses
// Tail-recursive version
void list_dump(node_t *node) {
  if (node == NULL) {
    return;
  } else {
    printf("Node at %p: data = %d next = %p\n",
        node, node->data, node -> next);
    // Recursive call
    list_dump(node->next);
  }
  return;
}

// Free memory of all nodes in a list
// Recursive version
// Note:
// The interface allows for writing e.g.
// mylist = list_free(mylist);
// which assigns a NULL to the pointer mylist right away.
// This helps in avoiding dangling pointers.
//
// Tail-recursive version
node_t* list_free(node_t* node) {
  if ( node != NULL) {
    node_t* tmp = node->next;
    // Free memory of current node
    free(node);
    // Recursive call
    list_free(tmp);
  }
  return NULL;
}

