// productor_impares.c
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
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int) * 100);
    int *data = mmap(NULL, sizeof(int) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Crear semáforo
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    // Generar números impares
    sem_wait(sem);
    for (int i = 0; i < 100; i++) {
        data[i] = (rand() % 50) * 2 + 1;
    }
    sem_post(sem);

    // Limpiar
    munmap(data, sizeof(int) * 5);
    close(shm_fd);
    sem_close(sem);

    return 0;
}