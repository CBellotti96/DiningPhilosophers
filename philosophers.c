#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;
pthread_cond_t cond_var;
int forks[NUM_PHILOSOPHERS];

void pickup_forks(int philosopher_number, int left_fork, int right_fork){
  pthread_mutex_lock(&mutex);
  while(forks[left_fork] || forks[right_fork])
    pthread_cond_wait(&mutex, &cond_var);
  printf("Philosopher %d taking forks and eating.\n", philosopher_number);
  forks[left_fork] = 1;
  forks[right_fork] = 1;
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopher_number, int left_fork, int right_fork){
  pthread_mutex_lock(&mutex);
  printf("Philosopher %d returning forks and thinking.\n", philosopher_number);
  forks[left_fork] = 0;
  forks[right_fork] = 0;
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&mutex);
}

void * philosopher(void *args){
  int *tmp = (int *)args;
  int i = tmp[0];
  int right = i;
  int left = (i - 1 == -1) ? NUM_PHILOSOPHERS - 1 : (i - 1);
  int loops_left = 10;
  while(loops_left > 0){
    pickup_forks(i, left, right);
    sleep(1);
    return_forks(i, left, right);
    sleep((random() % 3) + 1);
    loops_left--;
  }

}

int main(int argc, char**argv){
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_var, NULL);
  int i;
  for(i=0; i<NUM_PHILOSOPHERS; i++)
  forks[i] = 0;
  for(i=0; i<NUM_PHILOSOPHERS; i++){
    int *arg = malloc(sizeof(*arg));
    *arg = i;
    pthread_create( &philosophers[i], NULL, philosopher, arg);
  }
  for(i=0; i<NUM_PHILOSOPHERS; i++)
    pthread_join(philosophers[i],NULL);
  return 0;
}
