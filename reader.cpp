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
  key_t key = 1000;
  sem_init(&memory->mutex, 1, 1);
  int shmid;
  if ((shmid = shmget(key, sizeof(struct sharedMemory) + STRING_SIZE, 0644 | IPC_CREAT)) == -1) {
    perror("shmget");
    exit(1);
  }
  memory = (sharedMemory *)shmat(shmid, NULL, 0);
  if (memory == (sharedMemory *)(-1)) {
    perror("shmat");
    exit(1);
  }
  while (true) {
    sem_wait(&memory->mutex);
    int option = getIntRandom(4);
    if (option == 0) { //Imprimir las ordernes por año
      cout << "Imprimir las ordenes por año" << endl;
    } else if (option == 1) { // Imprimiendo todas las ordenes
      cout << "Imprimiendo Todas las ordenes" << endl << endl;
      for (int i = 0; i <= memory->counter; i++) {
        cout << "Orden: " << memory->array[i].orden << endl;
        cout << "Cliente: " << memory->array[i].client << endl;
        cout << "Cantidad de productos: " << memory->array[i].products << endl;
        cout << "Total: " << memory->array[i].total << endl << endl;
      }
    } else if (option == 2) { // Calcular el total de todas las ordenes
      double sum = 0;
      for (int i = 0; i <= memory->counter; i++) {
        sum += memory->array[i].total;
      }
      cout << "El total de las ordenes es " << sum << endl;
    } else { // Imprimir las ordenes de un cliente
      char client [STRING_SIZE];
      strcpy(client, clients[getIntRandom(5)]);
      cout << "Cliente: " << client << endl;
      int counter = 0;
      for (int i = 0; i <= memory->counter; i++) {
        if (memory->array[i].client == client) {
          cout << "Orden: " << memory->array[i].orden << endl;
          cout << "Cantidad de productos: " << memory->array[i].products << endl;
          cout << "Total: " << memory->array[i].total << endl << endl;
          counter++;
        }
      }
      if (counter == 0) {
        cout << "Ese cliente no tiene orden" << endl << endl;
      }
    }
    sem_post(&memory->mutex);
    sleep(2);
  }
  return 0;
}