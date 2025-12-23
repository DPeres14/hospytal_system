#ifndef IPC_H
#define IPC_H

#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

// ============================================================================
// NAMED PIPES (FIFOs) - 5 pipes
// ============================================================================
#define PIPE_INPUT              "input_pipe"
#define PIPE_TRIAGE             "triage_pipe"
#define PIPE_SURGERY            "surgery_pipe"
#define PIPE_PHARMACY           "pharmacy_pipe"
#define PIPE_LAB                "lab_pipe"
#define PIPE_PERMISSIONS        0666

// ============================================================================
// POSIX SEMAPHORES - 7 semáforos
// ============================================================================
#define SEM_SURGERY_BO1         "/sem_surgery_bo1"
#define SEM_SURGERY_BO2         "/sem_surgery_bo2"
#define SEM_SURGERY_BO3         "/sem_surgery_bo3"
#define SEM_MEDICAL_TEAMS       "/sem_medical_teams"
#define SEM_LAB1_EQUIPMENT      "/sem_lab1_equipment"
#define SEM_LAB2_EQUIPMENT      "/sem_lab2_equipment"
#define SEM_PHARMACY_ACCESS     "/sem_pharmacy_access"

// Valores iniciais dos semáforos
#define SEM_BO_INIT_VALUE       1    // Uma sala por vez
#define SEM_TEAMS_INIT_VALUE    2    // 2 equipas médicas
#define SEM_LAB_INIT_VALUE      2    // 2 slots simultâneos por lab
#define SEM_PHARMACY_INIT_VALUE 4    // 4 pedidos simultâneos

// Tipos de mensagens
typedef enum {
    MSG_PATIENT_ARRIVAL,
    MSG_TRIAGE_COMPLETE,
    MSG_LAB_REQUEST,
    MSG_LAB_RESULT,
    MSG_PHARMACY_REQUEST,
    MSG_PHARMACY_DELIVERY,
    MSG_SURGERY_REQUEST,
    MSG_SURGERY_COMPLETE,
    MSG_PATIENT_DISCHARGE
} MessageType;

// Estrutura de mensagem genérica
typedef struct {
    long msg_priority;      // 1=urgent, 2=high, 3=normal

    // Cabeçalho
    int msg_type;           // Ver códigos abaixo
    char source[20];        // \"TRIAGE\", \"SURGERY\", etc
    char target[20];        // Componente destino
    char patient_id[15];
    int operation_id;
    time_t timestamp;

    // Dados (formato texto simples ou JSON)
    char data[512];
} hospital_message_t;

// Tipos de mensagem
#define MSG_NEW_EMERGENCY       1
#define MSG_NEW_APPOINTMENT     2
#define MSG_NEW_SURGERY         3
#define MSG_PHARMACY_REQUEST    4
#define MSG_LAB_REQUEST         5
#define MSG_PHARMACY_READY      6
#define MSG_LAB_RESULTS_READY   7
#define MSG_CRITICAL_STATUS     8
#define MSG_TRANSFER_PATIENT    9
#define MSG_REJECT_PATIENT      10
// Estrutura de fila de mensagens para laboratório
typedef struct {
    long msg_type;
    int patient_id;
    int lab_id;
    char test_type[64];
} LabRequest;

// Estrutura de fila de mensagens para farmácia
typedef struct {
    long msg_type;
    int patient_id;
    char medicine_name[64];
    int quantity;
} PharmacyRequest;

// Estrutura de fila de mensagens para bloco operatório
typedef struct {
    long msg_type;
    int patient_id;
    int surgery_type;
    int estimated_duration;
} SurgeryRequest;

// Funções Essenciais - Message Queues
int send_message(int mqid, hospital_message_t *msg);
int receive_message_priority(int mqid, hospital_message_t *msg);
int create_all_message_queues(void);
void cleanup_message_queues(void);

// Funções Essenciais - Shared Memory
int create_all_shared_memory(void);
void* attach_shared_memory(key_t key, size_t size);
void initialize_shm_mutexes(void *shm);
void cleanup_shared_memory(void);

// Funções Essenciais - Named Pipes (FIFOs)
int create_all_pipes(void);
int open_pipe_read(const char *pipe_name);
int open_pipe_write(const char *pipe_name);
void cleanup_pipes(void);

// Funções Essenciais - POSIX Semaphores
int create_all_semaphores(void);
sem_t* open_semaphore(const char *sem_name, int initial_value);
int semaphore_wait(sem_t *sem);
int semaphore_post(sem_t *sem);
void cleanup_semaphores(void);

#endif // IPC_H
