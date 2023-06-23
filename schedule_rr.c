// ----------------------------------------------------------------------------
// Project 3: Scheduling Algorithms
// Author: Noah Nguyen
// Created: April 26, 2023
// ----------------------------------------------------------------------------
// schedule_rr.c:
//      - Round-robin (RR) scheduling, where each task is run for a time quantum
//      (or for the remainder of its CPU burst).
//      - For round-robin scheduling, the length of a time quantum is 10
//      milliseconds.
// ----------------------------------------------------------------------------
#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define the time quantum */
const int TIME_QUANTUM = 10;

// These pointers are used to keep track of the start and end of the linked list
/* initialize the head of the ready queue */
struct node *g_head = NULL;
/* initialize the tail of the ready queue */
struct node *g_tail = NULL;

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
  // if there are already tasks in queue, put the new task at the tail of the
  // queue
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
    struct node *temp;
  temp = g_head;
  // start running processes in the order they come 
  while (temp != NULL) {
    int run_time;
    Task* task = temp->task;
    // check for time quantum vs real process CPU burst time
    if (task->burst > TIME_QUANTUM) {
      run_time = TIME_QUANTUM;
    } else {
      run_time = task->burst;
    }

    // run the process in the run time
    run(task, run_time);
    time += run_time;
    printf("\tTime is now: %d\n", time);
    // check for leftover burst time
    task->burst -= run_time;

    // push the process to the end of the queue if it hasnt finished
    if (task->burst > 0) {
      // Create a new task to add back to the queue
      Task *new_task = malloc(sizeof(Task));
      new_task->name = task->name;
      new_task->priority = task->priority;
      new_task->burst = task->burst;

      // Add the unfinished task back to the queue
      // create a new node
      struct node *new_node = malloc(sizeof(struct node));
      new_node->task = new_task;
      new_node->next = NULL;

      // if the ready queue is empty, initialize the head and tail
      if (g_head == NULL) {
        g_head = new_node;
        g_tail = new_node;
      }
      // if there are already tasks in the queue, put the new task at the tail
      // of the queue
      else {
        g_tail->next = new_node;
        g_tail = new_node;
      }
    }

    free(task);
    temp = temp->next;
  }
}