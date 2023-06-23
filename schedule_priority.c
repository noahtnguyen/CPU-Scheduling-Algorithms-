// ----------------------------------------------------------------------------
// Project 3: Scheduling Algorithms
// Author: Noah Nguyen
// Created: April 26, 2023
// ----------------------------------------------------------------------------
// schedule_priority.c:
//      - Priority scheduling, which schedules tasks based on priority.
// ----------------------------------------------------------------------------
#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// These pointers are used to keep track of the start and end of the linked list
/* initialize the head of the ready queue */
struct node *g_head = NULL;
/* initialize the tail of the ready queue */
struct node *g_tail = NULL;

/* compare two characters */
bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

/* based on traverse from list.c
   finds the task whose name comes first in dictionary
*/
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  // find the task that has name that is lexicographical first among all the
  // tasks that have the highest priority.
  while (temp != NULL) {
    // check for higher priority
    // if they have equal priority, check for lexicographical order
    if ((temp->task->priority > best_sofar->priority) ||
        (temp->task->priority == best_sofar->priority &&
         comesBefore(temp->task->name, best_sofar->name))) {
      best_sofar = temp->task;
    }
    // move to the next task item
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

/* add a task to the ready queue */
void add(char *name, int priority, int burst) {
  // create a task that hold the attributes in params
  Task *task = malloc(sizeof(Task));
  task->name = name;
  task->priority = priority;
  task->burst = burst;

  // if the ready queue is empty. initialize its tail and head
  //  by allocating memory for the head and tail nodes using malloc.
  if (g_head == NULL) {
    g_head = malloc(sizeof(struct node));
    g_tail = malloc(sizeof(struct node));
    // set the task at g_head
    g_head->task = malloc(sizeof(struct task));
    g_head->task = task;
    g_head->next = NULL;
    // the tail now is also pointing at the head
    // since there is only 1 task in queue
    g_tail = g_head;
  }
  // otherwise, traverse through the list
  // and insert based on priority
  else {
    struct node *cur = g_head;
    struct node *prev = NULL;
    // find the right place to insert
    while (cur != NULL && cur->task->priority >= task->priority) {
      prev = cur;
      cur = cur->next;
    }

    // create a node to hold the new task
    struct node *new_node = malloc(sizeof(struct node));
    new_node->task = task;
    new_node->next = NULL;

    // perform insertion at node prev (ie. in the middle of the list)
    if (prev != NULL) {
      // insert: prev -> new_node -> the_rest
      new_node->next = prev->next;
      prev->next = new_node;
      // if the node is added at the end, update g_tail
      if (prev == g_tail) {
        g_tail = new_node;
      }
    }
    // the new task has higher priority than the g_head
    else {
      new_node->next = g_head;
      g_head = new_node;
    }
  }
}

/* performing the scheduling algorithm */
void schedule() {
  int time = 0;
  // start from the head of the queue
  // using pickNextTask() to pick which one comes first
  Task *task = pickNextTask();
  while (task != NULL) {
    run(task, task->burst);
    // get the time
    time += task->burst;
    printf("\tTime is now: %d\n", time);
    // free current task when done
    free(task);
    // pick the next task in queue
    task = pickNextTask();
  }
}