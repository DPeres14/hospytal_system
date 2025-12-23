#ifndef STATS_H
#define STATS_H

#include <time.h>
#include <pthread.h>

// SHM1: Estatísticas Globais
typedef struct {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;
    // Triagem
    int total_emergency_patients;
    int total_appointments;
    double total_emergency_wait_time;
    double total_appointment_wait_time;
    int completed_emergencies;
    int completed_appointments;
    int critical_transfers;
    int rejected_patients;
    // Blocos Operatórios
    int total_surgeries_bo1;
    int total_surgeries_bo2;
    int total_surgeries_bo3;
    double total_surgery_wait_time;
    int completed_surgeries;
    int cancelled_surgeries;
    double bo1_utilization_time;
    double bo2_utilization_time;
    double bo3_utilization_time;
    // Farmácia
    int total_pharmacy_requests;
    int urgent_requests;
    int normal_requests;
    double total_pharmacy_response_time;
    int stock_depletions;
    int auto_restocks;
    // Laboratórios
    int total_lab_tests_lab1;
    int total_lab_tests_lab2;
    int total_preop_tests;
    double total_lab_turnaround_time;
    int urgent_lab_tests;
    // Globais
    int total_operations;
    int system_errors;
    time_t system_start_time;
    int simulation_time_units;
} global_statistics_t;

// Estrutura de estatísticas
typedef struct {
    pthread_mutex_t mutex;
    
    // Estatísticas gerais
    int total_patients_processed;
    int total_critical_patients;
    int total_emergency_patients;
    int total_appointment_patients;
    
    // Estatísticas de triagem
    int triaged_patients;
    double avg_triage_time;
    int max_triage_time;
    int min_triage_time;
    
    // Estatísticas de laboratório
    int total_lab_tests;
    int lab1_tests;
    int lab2_tests;
    double avg_lab_time;
    
    // Estatísticas de farmácia
    int total_medicines_dispensed;
    int total_restocks;
    int medicines_with_low_stock;
    
    // Estatísticas de cirurgias
    int total_surgeries;
    int surgeries_completed;
    double avg_surgery_time;
    int max_surgery_time;
    int min_surgery_time;
    
    // Tempos médios
    double avg_patient_time_in_hospital;
    
    // Timestamp da última atualização
    time_t last_update;
} Statistics;

// Funções de estatísticas
int init_statistics(void);
void update_patient_triage_time(int patient_id, int triage_time);
void update_patient_surgery_time(int patient_id, int surgery_time);
void update_medicine_dispensed(const char *medicine_name, int quantity);
void update_lab_test(int lab_id, int test_duration);
void increment_patient_count(int priority);
Statistics *get_statistics(void);
int save_statistics_snapshot(const char *filename);
int close_statistics(void);

#endif // STATS_H
