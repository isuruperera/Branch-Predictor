//
// Created by isurutee on 6/25/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"

// the list element node structure. Add more stuff if you want to.
struct node {
    long id; // ID
    struct node *next; // link pointer (where the next element is)
};

// this allows us to refer to the above structure as a node.

// This defines *head as our top element. This allows us to find our
// begining element in the linked list.
struct node *head;

void linkedlist_addNode(long id) {
    struct node *curr = NULL;
    struct node *new = NULL;

    // Create and setup our new link
    new = malloc(sizeof(struct node));
    if (new == NULL) {
        printf("Memory not available to create link. Exiting.\\n");
        exit (EXIT_FAILURE);
    }
    new->id = id;

    // If Head is NULL, the list doesn’t yet exist, so we create one.
    if (head == NULL) {
        head = new;
        new->next = NULL;
        return;
    }

    if (new->id < head->id) {
        new->next = head;
        head = new;
        return;
    }

    curr = head;
    while ( (curr->next != NULL) ) {
        if (new->id < curr->next->id) {
            if((new->id > curr->id)) {
                new->next = curr->next;
                curr->next = new;
                return;
            } else {
                free(new); // get rid of ‘new’, we don’t need it.
                return;
            }
        }
        curr = curr->next; // move to the next node.
    }

    if (curr->id == new->id) {
        free(new);
        return;
    } else {
        curr->next = new;
        new->next = NULL;
        return;
    }
}

int linkedlist_getNodeCount() {
    struct node * temp;
    temp = head;
    int count = 0;
    while(temp) {
        count++;
        temp = temp->next ;
    }
    return count;
}

void linkedlist_clear() {
    struct node *temp;
    int count = 0;
    while(temp) {
        struct node *tempdel = temp;
        free(tempdel);
        temp = temp->next ;
    }
}

