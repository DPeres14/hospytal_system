#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <time.h>
#include <pthread.h>

#define INPUT_PIPE "input_pipe"

// Estados do paciente
typedef enum {
    PATIENT_CREATED,
    PATIENT_IN_TRIAGE,
    PATIENT_EMERGENCY,
    PATIENT_APPOINTMENT,
    PATIENT_LAB_TEST,
    PATIENT_PHARMACY,
    PATIENT_SURGERY,
    PATIENT_DISCHARGED
} PatientState;

// Prioridades de triagem
typedef enum {
    PRIORITY_CRITICAL,
    PRIORITY_EMERGENCY,
    PRIORITY_APPOINTMENT
} TriagePriority;

typedef enum {
    TRIAGE_1 = 1,   // mais crítico
    TRIAGE_2,
    TRIAGE_3,
    TRIAGE_4,
    TRIAGE_5        // menos crítico
} TriageLevel;

// Estrutura do Paciente
typedef struct {
    char patient_id[15];
    time_t arrival_time;
    time_t triage_start;
    int triage_level;   // 1-5
    int is_critical;   // 0/1 (quando estabilidade < limite)
    time_t triage_end;
    PatientState state;
    TriagePriority priority;
    int stability;          // 100-1000
    int critical_threshold; // normalmente 50
    int requires_lab_test;
    int requires_pharmacy;
    int requires_surgery;
    char medical_notes[256];
} Patient;

// Estrutura do Centro de Triagem
#define MAX_TRIAGE_PATIENTS 150

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    Patient emergency_queue[MAX_TRIAGE_PATIENTS];
    int emergency_count;

    Patient appointment_queue[MAX_TRIAGE_PATIENTS];
    int appointment_count;

    int active_patients;
    int total_triaged;
} TriageCenter;

// SHM2: Estado Blocos Operatórios
typedef struct {
    int room_id;
    int status;                 // 0=FREE, 1=OCCUPIED, 2=CLEANING
    char current_patient[15];
    int surgery_start_time;
    int estimated_end_time;
    pthread_mutex_t mutex;
} surgery_room_t;

typedef struct {
    surgery_room_t rooms[3];
    int medical_teams_available;
    pthread_mutex_t teams_mutex;
} surgery_block_shm_t;

// Estrutura do Bloco Operatório
typedef struct {
    int bo_id;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int in_use;
    int total_surgeries;
    time_t start_time;
    int duration;
} OperatingBlock;

// SHM3: Stock Farmácia
typedef struct {
    char name[30];
    int current_stock;
    int reserved;
    int threshold;
    int max_capacity;
    pthread_mutex_t mutex;
} medication_stock_t;

typedef struct {
    medication_stock_t medications[15];
    int total_active_requests;
    pthread_mutex_t global_mutex;
} pharmacy_shm_t;

// Estrutura da Farmácia
typedef struct {
    int medicine_id;
    char medicine_name[64];
    int stock;
    int threshold;
    int restock_count;
} Medicine;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Medicine medicines[15];
    int medicine_count;
    int total_dispensed;
    int total_restocks;
} Pharmacy;

// SHM4: Filas Laboratórios
typedef struct {
    char request_id[20];
    char patient_id[15];
    int test_type;
    int priority;
    int status;             // 0=pending, 1=processing, 2=done
    time_t request_time;
    time_t completion_time;
} lab_request_entry_t;

typedef struct {
    lab_request_entry_t queue_lab1[50];
    lab_request_entry_t queue_lab2[50];
    int lab1_count;
    int lab2_count;
    int lab1_available_slots; // 0-2
    int lab2_available_slots; // 0-2
    pthread_mutex_t lab1_mutex;
    pthread_mutex_t lab2_mutex;
} lab_queue_shm_t;

// Estrutura do Laboratório
typedef struct {
    int lab_id;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int active_tests;
    int total_tests;
    int simultaneous_capacity;
} Laboratory;

#endif // HOSPITAL_H
