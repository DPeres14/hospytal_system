#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include <semaphore.h>

// Estrutura de sincronização para filas
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    int capacity;
    int size;
} QueueSync;

// Estrutura de barrier para sincronizar threads
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int total;
    int generation;
} Barrier;

// Estrutura de semáforo para controlar acesso a recursos
typedef struct {
    sem_t semaphore;
    int initial_value;
} Semaphore;

// Estrutura de read-write lock
typedef struct {
    pthread_rwlock_t rwlock;
} ReadWriteLock;

// Funções de sincronização
int init_queue_sync(QueueSync *qs, int capacity);
int destroy_queue_sync(QueueSync *qs);

int init_barrier(Barrier *barrier, int count);
int wait_barrier(Barrier *barrier);
int destroy_barrier(Barrier *barrier);

int init_semaphore(Semaphore *sem, int value);
int post_semaphore(Semaphore *sem);
int wait_semaphore(Semaphore *sem);
int destroy_semaphore(Semaphore *sem);

int init_rwlock(ReadWriteLock *rwl);
int read_lock(ReadWriteLock *rwl);
int read_unlock(ReadWriteLock *rwl);
int write_lock(ReadWriteLock *rwl);
int write_unlock(ReadWriteLock *rwl);
int destroy_rwlock(ReadWriteLock *rwl);

#endif // SYNC_H
