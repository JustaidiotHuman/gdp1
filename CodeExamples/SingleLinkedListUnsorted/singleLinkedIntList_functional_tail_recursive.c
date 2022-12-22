// Implementation for unsorted single linked list of integers:
// We provide iterative and tail-recursive versions
//
// Demonstrate, that the compiler generates a tight loop for list_insert_end_aux, if
// the optimizer flag -O2 for gcc is specified.
//
// Configure
//    CFLAGS = -g -Wall -O2
// in the Makefile
//
// make clean; make
// gdb -q  -ex "disassemble list_insert_end_aux_V1" -ex "quit" bin/singleLinkedIntList_functional_tail_recursive_demo 
//
// gdb -q  -ex "disassemble list_insert_iter" -ex "quit" bin/singleLinkedIntList_functional_tail_recursive_demo
//
// NOTE:
// We use a very simple error handling method.
// If anything is wrong, we bail out with an exit()

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "singleLinkedIntList_type.h"

// forward decls

extern node_t* list_insert_end_V1(node_t*, int);
extern node_t* list_insert_end_aux_V1(node_t*, int, node_t*, node_t*);

extern node_t* list_insert_end_V1opt(node_t*, int);
extern node_t* list_insert_end_aux_V1opt(node_t*, int, node_t*, node_t*);

extern node_t* list_insert_end_iter(node_t*, int);

extern node_t* list_create_node(int data);
extern void list_dump(node_t *node);
extern node_t* list_free(node_t* node);

// --------------------------------------------------------------
// Some variants for inserting data at the end of a linked list
// --------------------------------------------------------------

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

// Dispatcher for the various versions

node_t* list_insert_end(node_t* node, int data) {
  return list_insert_end_V1(node,data);
  //return list_insert_end_V1opt(node,data);
  //return list_insert_end_iter(node,data);
}

// ----------------------------------------------------
// Non optimized tail recursive version V1
// ----------------------------------------------------

// A wrapper for the tail recursive version with the usual functional interface.
// Without optimization in the wrapper

node_t* list_insert_end_V1(node_t* node, int data) {
    return list_insert_end_aux_V1(node, data, node, node);
}

// Invariant inv: cur != NULL implies first != NULL && last != NULL
node_t* list_insert_end_aux_V1(node_t* cur, int data, node_t* first, node_t* last) {
  if (first == NULL){
      return list_create_node(data);
  }

  // By condition above and (inv): cur, first, last are all != NULL
  if (cur->next != NULL){
      // tail recursive call
      return list_insert_end_aux_V1(cur->next, data, first, cur);
  }

  // cur->next == NULL
  // We are at the last node
  cur -> next = list_create_node(data);
  // Return the anchor to the list
  return first;
}

// ----------------------------------------------------
// Iterative version
// ----------------------------------------------------
// Compare to list_insert_end_aux_V1
node_t* list_insert_end_iter(node_t* first, int data) {
  // Is the list empty
  if (first == NULL) {
    return list_create_node(data);
  }
  // List is not empty: go to last node
  node_t* cur = first;
  while (cur->next != NULL) {
    cur = cur -> next;
  }
  // cur->next == NULL
  // We are at the last node
  cur -> next = list_create_node(data);
  // Return the anchor to the list
  return first;
}

// ----------------------------------------------------
// Tail-recursive version: with slight optimization (*)
// ----------------------------------------------------

// A wrapper for the tail recursive version with the usual functional interface
// with slight optimization (*)
// (*) The condition that checks of the completely empty list
//     is moved into the wrapper.
node_t* list_insert_end_V1opt(node_t* node, int data) {
  if (node == NULL) {
    return list_create_node(data);
  }
  
  return list_insert_end_aux_V1opt(node->next, data, node, node);
}

// With -O2 optimization the compiler will generate a tight loop from this code
//
// Invariant due to (*) : first != NULL && last != NULL
node_t* list_insert_end_aux_V1opt(node_t* cur, int data, node_t* first, node_t* last) {
  if (cur == NULL){
    last -> next = list_create_node(data);
    return first;
  }

  return list_insert_end_aux_V1opt(cur->next, data, first, cur);
}

//--------------------------------------------------------------
// Additional functions, which are not in the focus of this demo
//--------------------------------------------------------------

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

