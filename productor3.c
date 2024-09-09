// productor_flotantes.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"

int main() {
    // Crear memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100);
    void *data = mmap(NULL, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Crear semáforo
    sem_t *sem = sem_open(SEM_NAME, 0);

    // Generar números flotantes
    sem_wait(sem);
    float *numbers_float = data + sizeof(int) * 200 + sizeof(char) * 100;
    for (int i = 0; i < 100; i++) {
        numbers_float[i] = (float)(rand() % 1000) / 10.0;  // Números flotantes aleatorios
    }
    sem_post(sem);

    // Limpiar
    munmap(data, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100);
    close(shm_fd);
    sem_close(sem);

    return 0;
}