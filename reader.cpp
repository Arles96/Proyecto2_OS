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
  int shmid;
  if ((shmid = shmget(key, sizeof(struct sharedMemory), 0644)) == -1) {
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
    if (memory->counter > 0) {
      sem_wait(&memory->mutex);
      int option = getIntRandom(4);
      if (option == 0) { //Imprimir las ordernes por año
        cout << "Imprimir las ordenes por año" << endl;
      } else if (option == 1) { // Imprimiendo todas las ordenes
        cout << "Imprimiendo Todas las ordenes" << endl << endl;
        for (int i = 0; i <= memory->counter; i++) {
          cout << "Orden: " << memory->orden[i] << endl;
          cout << "Cliente: " << memory->client[i] << endl;
          cout << "Cantidad de productos: " << memory->products[i] << endl;
          cout << "Total: " << memory->total[i] << endl << endl;
        }
      } else if (option == 2) { // Calcular el total de todas las ordenes
        double sum = 0;
        for (int i = 0; i <= memory->counter; i++) {
          sum += memory->total[i];
        }
        cout << "El total de las ordenes es " << sum << endl;
      } else { // Imprimir las ordenes de un cliente
        char client [STRING_SIZE];
        strcpy(client, clients[getIntRandom(5)]);
        cout << "Cliente: " << client << endl;
        int counter = 0;
        for (int i = 0; i <= memory->counter; i++) {
          if (memory->client[i] == client) {
            cout << "Orden: " << memory->orden[i] << endl;
            cout << "Cantidad de productos: " << memory->products[i] << endl;
            cout << "Total: " << memory->total[i] << endl << endl;
            counter++;
          }
        }
        if (counter == 0) {
          cout << "Ese cliente no tiene orden" << endl << endl;
        }
      }
      sem_post(&memory->mutex);
      sleep(1);
    }
  }
  return 0;
}