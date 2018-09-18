#include <pthread.h>
#include <iostream>
#include <string.h>
#include <semaphore.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

#define STRING_SIZE 20
#define TABLE_SIZE 50

struct table {
  int orden;
  int products;
  double total;
  char client[STRING_SIZE];
  char date [STRING_SIZE] ;
};

int getIntRandom (int range) {
  srand(time(NULL));
  return rand() % range;
}

double getDoubleRandom (int range, double min, double max) {
  srand(time(NULL));
  double number = (double) rand() / range;
  return min + number * (max - min);
}

struct sharedMemory {
  table array [TABLE_SIZE];
  int counter = 0;
  sem_t mutex;
};

sharedMemory *memory;

int main () {
  string clients [5] = { "Dario", "Arles", "Cristhian", "Harold", "Ana" };
  key_t key = 5646;
  int shmid;
  if ((shmid = shmget(key, sizeof(struct sharedMemory), 0644 | IPC_CREAT)) == -1) {
    perror("shmget");
    exit(1);
  }
  return 0;
}