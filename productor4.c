#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"

int main() {
    // Inicializar la semilla para números aleatorios
    srand(time(NULL));

    // Crear memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Ajustar el tamaño de la memoria compartida
    ftruncate(shm_fd, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100);
    void *data = mmap(NULL, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100, PROT_READ 
    | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Crear semáforo
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Generar caracteres aleatorios
    sem_wait(sem);
    char *chars = (char *)(data + sizeof(int) * 200);
    for (int i = 0; i < 100; i++) {
        chars[i] = 'A' + (rand() % 26);  // Genera un carácter aleatorio entre 'A' y 'Z'
    }
    sem_post(sem);

    // Limpiar
    munmap(data, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100);
    close(shm_fd);
    sem_close(sem);

    return 0;
}

