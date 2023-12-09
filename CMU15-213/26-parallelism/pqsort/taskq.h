/* Create and reap queue of tasks */

typedef void * (*thread_routine_t)(void *);

/* Queue represented as singly-linked list of records */
typedef struct TELE *task_list_ptr;

typedef struct TELE {
  pthread_t tid;
  task_list_ptr next;
} task_list_t;

/* All data associated with task queue encapsulated as struct */
typedef struct {
  /* Number of tasks are in process */
  int active_count;
  int max_active_count;
  task_list_ptr head;
  /* Maintain pointer to tail of list for insertion */
  task_list_ptr tail;
  /* Mutex to protect all modifications to queue */
  pthread_mutex_t mutex;
#ifdef SEM
  /* Initial binary
     semaphore to make sure reaper only starts after first task spawned */
  sem_t reap_ok;
#endif
} task_queue_t, *task_queue_ptr;
  
task_queue_ptr new_task_queue();
void free_task_queue(task_queue_ptr tq);

/* Create a new task, spawning thread.  Return nonzero if successful */
void spawn_task(task_queue_ptr tq, thread_routine_t routine, void *tdata);

/* Join all tasks in queue.  Won't return until queue becomes empty.
   Returns maximum number of concurrrent tasks
*/
int join_tasks(task_queue_ptr tq);






    
