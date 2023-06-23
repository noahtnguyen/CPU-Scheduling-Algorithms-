// ----------------------------------------------------------------------------
// Project 3: Scheduling Algorithms
// Author: Noah Nguyen
// Created: April 26, 2023
// ----------------------------------------------------------------------------
// schedule_fcfs.c:
//      - First-come, first-served (FCFS),
//        which schedules tasks in the order in which they request the CPU.
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

  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
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
  // if there are already tasks in queue, put the new task at the tail of the queue 
  else {
    // create a new node
    struct node *new_node = malloc(sizeof(struct node));
    // the new node is now the tail because it is added at the end of the list
    g_tail->next = new_node;
    // set the node to hold the new task
    new_node->task = malloc(sizeof(struct task));
    new_node->task = task;
    new_node->next = NULL;
    g_tail = new_node;
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