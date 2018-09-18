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
#include <unistd.h>
#include <ctime>

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
char clients [5] [STRING_SIZE] = { "Dario", "Arles", "Cristhian", "Harold", "Ana" };

int main () {
  key_t key = 5646;
  int shmid;
  if ((shmid = shmget(key, sizeof(struct sharedMemory), 0644 | IPC_CREAT)) == -1) {
    perror("shmget");
    exit(1);
  }
  memory = (sharedMemory *)shmat(shmid, NULL, 0);
  if (memory == (sharedMemory *)(-1)) {
    perror("shmat");
    exit(1);
  }
  while (true) {
    int option = getIntRandom(2);
    if (option == 0) { // Agregando ordenes
      table *newTable;
      strcpy(newTable->client, clients[getIntRandom(5)]);
      newTable->orden = getIntRandom(2019);
      time_t now = time(0);
      strcpy(newTable->date, ctime(&now));
      newTable->products = getIntRandom(20);
      newTable->total = getDoubleRandom(500, 100, 499);
      memory->array[memory->counter] = *newTable;
      memory->counter++;
      cout << "Un escritor agrego una orden" << endl;
    } else { // eliminando ordenes
      memory->array[memory->counter] = NULL;
      memory->counter--;
      cout << "Un escritor ha eliminado una orden" << endl;
    }
    sleep(2);
  }
  return 0;
}