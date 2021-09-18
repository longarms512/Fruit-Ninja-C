//=================================================================
// Implementation for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

LLNode* create_llnode(void* data) {

    LLNode* new_node = (LLNode*)malloc(sizeof(LLNode));
    new_node->prev = NULL; // initialize nodes prev pointer to null
    new_node->next = NULL; // initialize nodes next pointer to null
    new_node->data = data; // initialize data pointer member to data
  
    return new_node;
}

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

int getSize(DLinkedList* dLinkedList){

    return dLinkedList->size; // return size element of that list
}

LLNode* getHead(DLinkedList* dLinkedList){

    if (dLinkedList->head != NULL) { // if the head is not null
        return (dLinkedList->head); // return the data of the head of the list
    }
    else {
        return NULL; // else return null
    }
}

LLNode* getTail(DLinkedList* dLinkedList){

    if (dLinkedList->tail != NULL) { // if the tail is not null
        return (dLinkedList->tail);  // return the data of the tail of the list
    }
    else {
        return NULL;
    }

}

LLNode* getNext(LLNode* node){

    if (node->next != NULL) { // check to see if there is a next element
        node = node->next; // make the current node pointer euqal to the next node
        return node; // return a pointer to the next node
    }
    else {
        return NULL;
    }

}

LLNode* getPrev(LLNode* node){

    if (node->prev != NULL) { // check to see if there is a prev pointer
        node = node->prev; // make the current node pointer equal to the prev node
        return node; // return the prev node
    }
    else {
        return NULL; // else return NULL
    }
}

void* getData(LLNode* node){

    return node->data; // return the data for that node 
}

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data){
    if (prev_node == NULL) { // check to see if the prev_node is NULL
        printf("the given previous node cannot be NULL");
        return;
    }

    int flag = 0; // flag variable

    LLNode* newNode = create_llnode(data); // create a new node with that data

    newNode->prev = prev_node; // make the prev node of the new node = pre_node
    newNode->data = data; // make the data = to data

    if (prev_node->next != NULL) { // check to see if there is an element after the current prev_node
        //printf("I am doing this\n");
        newNode->next = prev_node->next; // make the pointer to newNode next = prev node's next
        (prev_node->next)->prev = newNode; // make the pointer for the node after prev's = to the new Node
    }

    else {
        flag = 1; // make flag equal to 1 if the prev_node is the tail of the list 
    }

    prev_node->next = newNode; // make the prev node's next pointer to the newNode

    if (flag == 1) { // if the prev_node is the tail of the list
        //printf("A flag was recieved\n");
        dLinkedList->tail = (dLinkedList->tail)->next; // update the tail of the list
    }

    //printf("Completed insert after\n");

    dLinkedList->size++; // increase the size of the list by 1

    return;
}

void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data){
    if (next_node == NULL) { // check to see if the next node is NULL
        printf("the given next node cannot be NULL");
        return;
    }

    int flag = 0; // flag variable used later

    LLNode* newNode = create_llnode(data); // create a new node
    
    newNode->next = next_node; // make the newNode next pointer = next_node
    newNode->data = data; // make newNode data pointer = data

    if (next_node->prev != NULL) { // check to make sure that next_node is not the first element in the list
        newNode->prev = next_node->prev; // make the newNode prev pointer = next_nodes's prev
        (next_node->prev)->next = newNode; // make next_node's original prev's next pointer equal to newNode
    }
    else { 
        flag = 1; // if next_node is the first element in the list
    }

    next_node->prev = newNode; // make next_node's prev pointer = newNode

    if (flag == 1) { // if next node is the first element in the list
        dLinkedList->head = (dLinkedList->head)->prev;  // make the new head of the list the new Node
    }

    dLinkedList->size++; // increase the size of the linked list by 1

    return;
}

void insertHead(DLinkedList* dLinkedList, void* data){
  if(dLinkedList->head == NULL){
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++; 
  }else{
    insertBefore(dLinkedList, dLinkedList->head, data);
  }
}

void insertTail(DLinkedList* dLinkedList, void* data){

    if (dLinkedList->tail == NULL) { // if there are no elements in the linked list yet

        //printf("data is: %d: \n", data);

        LLNode* newNode = create_llnode(data);
        dLinkedList->head = newNode;
        dLinkedList->tail = newNode;
        dLinkedList->size++;
    }
    else { // if elements exist in the linkedList
        //printf("inserting after\n");
        insertAfter(dLinkedList, dLinkedList->tail, data);

    }

}

void deleteNode(DLinkedList* dLinkedList, LLNode* node){

    if (node == dLinkedList->head && node == dLinkedList->tail) {
        dLinkedList->head = NULL;
        dLinkedList->tail = NULL;
        free(node->data); // free the node
        free(node); // free the node
    }
    
    else if (node == dLinkedList->head) { // if there is not previous node pointer (aka is the node the head)

        //printf("This is the head\n");
        //printf("This is the node: %p\n", node->data);
        //printf("This is the next node: %p\n", node->next->data);

        dLinkedList->head = node->next; // make the head the next node
        (node->next)->prev = NULL; // make the next node's prev pointer NULL
        node->next = NULL; // make the nodes next pointer NULL
        free(node->data); // free the node
        free(node); // free the node
    }

    else if (node == dLinkedList->tail) { // if it is the last node (aka is the node the tail)

        //printf("This is the tail\n");

        dLinkedList->tail = node->prev; // make the tail of the list the prev node
        (node->prev)->next = NULL; // make the next pointer of the prev node NULL
        node->prev = NULL; // make node prev = NULL
        free(node->data); // free the node
        free(node); // free the node
    }

    else { // if it is in the middle of a linked list

        //printf("This is the middle of a lsit\n");

        (node->prev)->next = node->next; // make the prev's next to the current node's next
        (node->next)->prev = node->prev; // make the next's prev to the current node's prev
        free(node->data); // free the node
        free(node); // free the node
    }

    //printf("This is the new head of the list: %p\n", dLinkedList->head->data);

    
    dLinkedList->size--; // decrease the size
    return;

}

void destroyList(DLinkedList* dLinkedList){

    for (int i = 0; i < (dLinkedList->size--)- 1;) { // iterate for one less than the length of the list

        //printf("removed Head %d\n", i);

        dLinkedList->head = (dLinkedList->head)->next; // keep moving the head pointer to the next node
        (dLinkedList->head)->prev->data = NULL; // make the prev data pointer NULL;
        (dLinkedList->head)->prev->next = NULL; // make the prev next pointer NULL;
        (dLinkedList->head)->prev->prev = NULL; // make the prev prev pointer NULL;
        free((dLinkedList->head)->prev); // free the node previous to the head pointer
    }
    
    free(dLinkedList->head); // free the last node 

    free(dLinkedList); // free the list

    //printf("New Head is %d\n", dLinkedList->head);
    //printf("New Tail is %d\n", dLinkedList->tail);

    return;

}

void reverse(DLinkedList* dLinkedList)
{

    LLNode* temp = NULL; // create a temp node
    LLNode* current = getHead(dLinkedList); // create the current node
    dLinkedList->tail = dLinkedList->head; // set the tail of the list equal to the head of the list

    //int i = 0; // debugging

    while (current != NULL) {
        //printf("turn number: %d \n", i);
        temp = current->next; // set temp node
        current->next = current->prev; // set the next node to the prev node
        current->prev = temp; // set the prev node to the next node via the temp node
        dLinkedList->head = current; // make the head of the list the current node (keeps changing to the current node after current is updated)
        current = current->prev; // change current to the next node in the list (which is now current->prev)
        //i++; //debugging
    }

    //printf("the head of the list is: %d \n", dLinkedList->head);
    //printf("the tail of the list is: %d \n", dLinkedList->tail);
    

    return;

}     
