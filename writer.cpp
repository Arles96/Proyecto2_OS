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
  int orden[TABLE_SIZE];
  int products[TABLE_SIZE];
  double total[TABLE_SIZE];
  char client[TABLE_SIZE][STRING_SIZE];
  char date [TABLE_SIZE][STRING_SIZE] ;
  int counter = 0;
  sem_t mutex;
};

sharedMemory *memory;
char clients [5] [STRING_SIZE] = { "Dario", "Arles", "Cristhian", "Harold", "Ana" };

int main () {
  key_t key = 5656;
  //sem_init(&memory->mutex, 1, 1);
  int shmid;
  cout << "123" << endl;
  if ((shmid = shmget(key, sizeof(struct sharedMemory), 0644 | IPC_CREAT)) == -1) {
    perror("shmget");
    exit(1);
  }
  memory = (sharedMemory *)shmat(shmid, NULL, 0);
  if (memory == (sharedMemory *)(-1)) {
    perror("shmat");
    exit(1);
  }
  sem_init(&memory->mutex, 1, 1);
  while (true) {
    sem_wait(&memory->mutex);
    int option = getIntRandom(2);
    if (option == 0) { // Agregando ordenes
      cout << "Hola Agregando" << endl;
      strcpy(memory->client[memory->counter], clients[getIntRandom(5)]);
      cout << "copy" << endl;
      memory->orden[memory->counter] = getIntRandom(2019);
      time_t now = time(0);
      strcpy(memory->date[memory->counter], ctime(&now));
      memory->products[memory->counter] = getIntRandom(20);
      memory->total[memory->counter] = getDoubleRandom(500, 100, 499);
      memory->counter++;
      cout << "Un escritor agrego una orden" << endl;
      sleep(2);
    } else { // eliminando ordenes
      // memory->array[memory->counter] = NULL;
      if (memory->counter > 0) {
        memory->counter--;
        cout << "Un escritor ha eliminado una orden" << endl;
        sleep(2);
      }
    }
    sem_post(&memory->mutex);

  }
  return 0;
}