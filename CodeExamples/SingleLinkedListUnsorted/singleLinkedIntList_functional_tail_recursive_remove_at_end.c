// We provide deep recursive, tail-recursive and iterative versions for deleting at the end
//
// Demonstrate, that the compiler generates a tight loop for list_remove_tail_aux_V1, if
// the optimizer flag -O2 for gcc is specified.

// TODO
// make clean; make
// gdb -q  -ex "disassemble list_insert_end_aux_V1" -ex "quit" bin/singleLinkedIntList_functional_tail_recursive_demo 
//
// gdb -q  -ex "disassemble list_insert_end_iter" -ex "quit" bin/singleLinkedIntList_functional_tail_recursive_demo
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "singleLinkedIntList_type.h"

// variants for deleting at the end
extern node_t* list_remove_tail_deep_rec(node_t* node);
extern node_t* list_remove_tail_V1(node_t* node);
extern node_t* list_remove_tail_aux_V1(node_t* node, node_t* first, node_t* last);

// variants for inserting at the end
extern node_t* list_insert_end_deep_rec(node_t* node, int data);

extern node_t* list_insert_end_V1opt(node_t*, int);
extern node_t* list_insert_end_aux_V1opt(node_t*, int, node_t*, node_t*);

// Auxiliary functions
extern node_t* list_create_node(int data);
extern void list_dump(node_t *node);
extern node_t* list_free(node_t* node);

// --------------------------------------------------------------
// Some variants for deleting the element at the end of the list
// --------------------------------------------------------------

// Dispatcher for the various versions
node_t* list_remove_tail(node_t* node) {
  //return list_remove_tail_deep_rec(node);
  return list_remove_tail_V1(node);
}

// Remove tail node of list
// Deep recursive version
node_t* list_remove_tail_deep_rec(node_t* node){
  if (node == NULL) {
    return NULL;
  } else {
    if (node->next == NULL) {
      // Remove last node
      free(node);
      return NULL;
    } else {
      // Recursive call
      node->next = list_remove_tail_deep_rec(node->next);
      return node;
    }
  }
}

node_t* list_remove_tail_V1(node_t* node){
  return list_remove_tail_aux_V1(node, node, NULL);
}

// Invariant: node != NULL --> first != NULL   TODO
node_t* list_remove_tail_aux_V1(node_t* cur, node_t* first, node_t* last){
  if (cur == NULL){
    return NULL;
  } else {
    if (cur->next != NULL){
      // Tail recursive call
      return list_remove_tail_aux_V1(cur->next, first, cur);
    } else {
      // Remove last node
      free(cur);
      if (last == NULL){
        return NULL;
      } else {
        last -> next = NULL;
        return first;
      }
    }
  }
}

// TODO: add optimized version for V1
// TODO: add version V2 with if_else
// TODO: add iterative version

// --------------------------------------------------------------
// Some variants for inserting data at the end of a linked list
// --------------------------------------------------------------

// Dispatcher for the various versions
node_t* list_insert_end(node_t* node, int data) {
  //return list_insert_end_deep_rec(node,data);
  return list_insert_end_V1opt(node,data);
}

// Original deep recursive version
node_t* list_insert_end_deep_rec(node_t* node, int data) {
  // Is the list empty
  if (node == NULL) {
    return list_create_node(data);
  } else {
    // Recursive call
    node -> next = list_insert_end_deep_rec(node->next, data);
    return node;
  }
}

// ----------------------------------------------------
// Tail-recursive version: with slight optimization (*)
// ----------------------------------------------------

// A wrapper for the tail recursive version with the usual functional interface
// with slight optimization (*)
// (*) The condition that checks for the empty list
//     is moved into the wrapper.
node_t* list_insert_end_V1opt(node_t* node, int data) {
  if (node == NULL) {
    return list_create_node(data);
  }

  return list_insert_end_aux_V1opt(node->next, data, node, node);
}

// With -O2 optimization the compiler will generate a tight loop from this code
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

