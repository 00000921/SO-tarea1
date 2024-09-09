// consumidor.c
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

    // Compilar y ejecutar los productores en orden  
    system("gcc productor1.c -o productor1 && ./productor1");  
    system("gcc productor2.c -o productor2 && ./productor2");  
    system("gcc productor3.c -o productor3 && ./productor3");  
    system("gcc productor4.c -o productor4 && ./productor4");  

    // Abrir memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    void *data = mmap(NULL, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Abrir semáforo
    sem_t *sem = sem_open(SEM_NAME, 0);

    int *numbers_odd = data;
    int *numbers_even = data + sizeof(int) * 100;
    char *chars = (char *)(data + sizeof(int) * 200);
    float *numbers_float = (float *)(data + sizeof(int) * 200 + sizeof(char) * 100);

    // Imprimir datos de manera intercalada
    sem_wait(sem);
    for (int i = 0; i < 100; i++) {
        printf("%d\n", numbers_odd[i]);
        printf("%d\n", numbers_even[i]);
        printf("%.2f\n", numbers_float[i]);
        printf("%c\n", chars[i]);
    }
    sem_post(sem);

    // Limpiar
    munmap(data, sizeof(int) * 200 + sizeof(char) * 100 + sizeof(float) * 100);
    close(shm_fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);

    return 0;
}