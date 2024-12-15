#include <stdlib.h>
#include <stdio.h>

#include "pos.h"

pos make_pos(unsigned int r, unsigned int c) {
  pos p;
  p.r = r;
  p.c = c;
  return p;
}

posqueue* posqueue_new() {
  posqueue* q = (posqueue*)malloc(sizeof(posqueue));
  if (!q) {
    fprintf(stderr, "Error: Memory allocation error\n");
    exit(1);
  }
  q->head = NULL;
  q->tail = NULL;
  q->len = 0;
  return q;
}

void pos_enqueue(posqueue* q, pos p) {
  pq_entry* new = (pq_entry*)malloc(sizeof(pq_entry));
  if (!new) {
    fprintf(stderr, "Error: Memory allocation error\n");
    exit(1);
  }
  new->p = p;
  new->prev = q->tail;
  if (q->tail) {
    q->tail->next = new;
  }
  q->tail = new;
  new->next = NULL;
  if (!q->head) {
    q->head = new;
    q->head->prev = NULL;
  }
  q->len++;
}

pos pos_dequeue(posqueue* q) {
  if (!q->len) {
    fprintf(stderr, "Error: Cannot remove from an empty queue\n");
    exit(1);
  }
  pq_entry* new_head = q->head->next;
  if (new_head) {
    new_head->prev = NULL;
  }
  pos p = q->head->p;
  free(q->head);
  q->head = new_head;
  q->len--;
  if (!q->len) {
    q->tail = NULL;
  }
  return p;
}

pos posqueue_remback(posqueue* q) {
  if (!q->len) {
    fprintf(stderr, "Error: Cannot remove from an empty queue\n");
    exit(1);
  }
  pq_entry* new_tail = q->tail->prev;
  if (new_tail) {
    new_tail->next = NULL;
  }
  pos p = q->tail->p;
  free(q->tail);
  q->tail = new_tail;
  q->len--;
  if (!q->len) {
    q->head = NULL;
  }
  return p;
}

void posqueue_free(posqueue* q) {
  if (!q) {
    return;
  }
  pq_entry *current = q->head, *next;
  while (current) {
    next = current->next;
    free(current);
    current = next;
  }
  free(q);
}
