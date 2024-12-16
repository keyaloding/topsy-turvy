#ifndef POS_H
#define POS_H

/* Represents a position on the board. */
struct pos {
  unsigned int r, c;
};

typedef struct pos pos;

typedef struct pq_entry pq_entry;

/* Entries within a `posqueue` that store the position. */
struct pq_entry {
  pos p;
  pq_entry *next, *prev;
};

/* A doubly linked list that stores a player's pieces. */
struct posqueue {
  pq_entry *head, *tail;
  unsigned int len;
};

typedef struct posqueue posqueue;


/* Creates a `pos`. Does not verify that the coordinates are valid. */
pos make_pos(unsigned int r, unsigned int c);

/* Allocates a new, empty queue. */
posqueue* posqueue_new();

/* Adds a position to the back of a queue. */
void pos_enqueue(posqueue* q, pos p);

/* Removes and returns the element at the front of the queue. Raises an error
 * if the queue is empty. */
pos pos_dequeue(posqueue* q);

/* Removes and returns the element at the back of the queue. Raises an error
 * if the queue is empty. */
pos posqueue_remback(posqueue* q);

/* Frees an existing queue from memory. */
void posqueue_free(posqueue* q);

#endif /* POS_H */
