#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include "pos.h"

Test(make_pos, test_new_pos) {
  pos p = make_pos(7, 11);
  cr_assert(p.r == 7);
  cr_assert(p.c == 11);
}

Test(posqueue_new, test_new_queue) {
  posqueue* q = posqueue_new();
  cr_assert(!q->len);
  cr_assert(!q->head);
  cr_assert(!q->tail);
  posqueue_free(q);
}

Test(pos_enqueue, test_len3_queue) {
  pos p1 = make_pos(3, 4), p2 = make_pos(6, 6), p3 = make_pos(5, 1);
  posqueue* q = posqueue_new();
  pos_enqueue(q, p1);
  pos_enqueue(q, p2);
  pos_enqueue(q, p3);
  cr_assert(q->len == 3);
  cr_assert(q->head->p.r == 3);
  cr_assert(q->head->p.c == 4);
  cr_assert(q->tail->p.r == 5);
  cr_assert(q->tail->p.c == 1);
  posqueue_free(q);
}

Test(pos_dequeue, test_dequeue1) {
  posqueue* q = posqueue_new();
  pos p1 = make_pos(2, 19), p2 = make_pos(5, 4), p3 = make_pos(1, 0);
  pos_enqueue(q, p1);
  pos_enqueue(q, p2);
  pos_enqueue(q, p3);
  pos first = pos_dequeue(q);
  cr_assert(first.r == 2);
  cr_assert(first.c == 19);
  cr_assert(q->len == 2);
  posqueue_free(q);
}

Test(pos_dequeue, test_dequeue2) {
  posqueue* q = posqueue_new();
  pos p1 = make_pos(2, 19), p2 = make_pos(5, 4);
  pos_enqueue(q, p1);
  pos_enqueue(q, p2);
  pos first = pos_dequeue(q);
  cr_assert(first.r == 2);
  cr_assert(first.c == 19);
  cr_assert(q->len == 1);
  cr_assert(q->tail->p.c == 4);
  cr_assert(q->tail->p.r == 5);
  cr_assert(q->head->p.c == 4);
  cr_assert(q->head->p.r == 5);
  pos next = pos_dequeue(q);
  cr_assert(next.r == 5);
  cr_assert(next.c == 4);
  cr_assert(!q->len);
  posqueue_free(q);
}

Test(posqueue_remback, test_remove3) {
  posqueue* q = posqueue_new();
  pos p1 = make_pos(4, 3), p2 = make_pos(7, 11), p3 = make_pos(6, 2);
  pos_enqueue(q, p1);
  pos_enqueue(q, p2);
  pos_enqueue(q, p3);
  pos p4 = posqueue_remback(q);
  cr_assert(p4.r == 6);
  cr_assert(p4.c == 2);
  pos p5 = posqueue_remback(q);
  cr_assert(p5.r == 7);
  cr_assert(p5.c == 11);
  pos p6 = posqueue_remback(q);
  cr_assert(p6.r == 4);
  cr_assert(p6.c == 3);
  cr_assert(!q->len);
  posqueue_free(q);
}
