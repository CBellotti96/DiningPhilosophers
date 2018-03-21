//Chris Bellotti

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PHILOSOPHERS 5

pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;
pthread_cond_t cond_var;
int forks[NUM_PHILOSOPHERS];

void pickup_forks(int philosopher_number, int left_fork, int right_fork){
  pthread_mutex_lock(&mutex);
  while(forks[left_fork] || forks[right_fork]) //if either forks are taken wait until they're free
    pthread_cond_wait(&cond_var, &mutex);
  printf("Philosopher %d taking forks and eating.\n", philosopher_number);
  fflush(stdout);
  forks[left_fork] = 1;
  forks[right_fork] = 1;
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopher_number, int left_fork, int right_fork){
  pthread_mutex_lock(&mutex);
  printf("Philosopher %d returning forks and thinking.\n", philosopher_number);
  fflush(stdout);
  forks[left_fork] = 0;
  forks[right_fork] = 0;
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&mutex);
}

void * philosopher(void *args){
  int *tmp = (int *)args;
  int i = tmp[0];
  printf("Philosopher %d ready to eat and think.\n", i);
  fflush(stdout);
  int right = i;
  int left = (i - 1 == -1) ? NUM_PHILOSOPHERS - 1 : (i - 1);
  int loops_left = 3;
  while(loops_left > 0){ //looping so each philosopher eats 3 times before program ends
    pickup_forks(i, left, right);
    sleep(1);
    return_forks(i, left, right);
    sleep((random() % 3) + 1); //think for a while after eating
    loops_left--;
  }

}

int main(int argc, char**argv){
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_var, NULL);
  int i;
  for(i=0; i<NUM_PHILOSOPHERS; i++) //initializing forks
  forks[i] = 0;
  for(i=0; i<NUM_PHILOSOPHERS; i++){ //creating thread for each philosopher
    int *arg = malloc(sizeof(*arg));
    *arg = i;
    pthread_create( &philosophers[i], NULL, philosopher, arg);
  }
  for(i=0; i<NUM_PHILOSOPHERS; i++)
    pthread_join(philosophers[i],NULL); //join threads at the end and exit
  return 0;
}
