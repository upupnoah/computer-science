#include "csapp.h"
#include "taskq.h"

/* Create and reap queue of tasks */
task_queue_ptr new_task_queue() {
  task_queue_ptr tq = Malloc(sizeof(task_queue_t));
  tq->active_count = tq->max_active_count = 0;
  tq->head = tq->tail = NULL;
  pthread_mutex_init(&tq->mutex, NULL);
#ifdef SEM
  sem_init(&tq->reap_ok, 0, 0);
#endif
  return tq;
}


void free_task_queue(task_queue_ptr tq) {
  /* Assumes that queue is already empty */
  free((void *) tq);
}

/* Create a new task, spawning thread.  Return nonzero if successful */
void spawn_task(task_queue_ptr tq, thread_routine_t routine, void *tdata) {
  int first_task = 0;
  /* Create new task element */
  task_list_ptr ele = Malloc(sizeof(task_list_t));
  ele->next = NULL;
  /* Acquire lock to queue state */
  pthread_mutex_lock(&tq->mutex);
  first_task = tq->max_active_count == 0;
  if (tq->tail)
    tq->tail->next = ele;
  else
    tq->head = ele;
  tq->tail = ele;
  tq->active_count++;
  if (tq->active_count > tq->max_active_count)
    tq->max_active_count = tq->active_count;
  Pthread_create(&ele->tid, NULL, routine, tdata);
#ifdef SEM
  if (first_task)
    sem_post(&tq->reap_ok);
#endif
  pthread_mutex_unlock(&tq->mutex);
}



/* Join all tasks in queue.  Won't return until queue becomes empty.
   Returns maximum number of concurrrent tasks
*/
int join_tasks(task_queue_ptr tq) {
#ifdef SEM
  /*
    This may be needed at the very beginning
    in case no tasks have been queued yet
  */
  P(&tq->reap_ok);
#endif
  /* Basic loop:
     Assumes that queue won't become empty until all tasks completed. */
  while (1) {
    pthread_mutex_lock(&tq->mutex);
    task_list_ptr old_head = tq->head;
    if (old_head == NULL) {
      pthread_mutex_unlock(&tq->mutex);
      return tq->max_active_count;
    } else {
      pthread_t tid = old_head->tid;
      tq->head = old_head->next;
      if (old_head == tq->tail)
	tq->tail = NULL;
      tq->active_count--;
      pthread_mutex_unlock(&tq->mutex);
      free((void *) old_head);
      Pthread_join(tid, NULL);
    }
  }
}












    
