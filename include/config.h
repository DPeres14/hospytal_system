#ifndef CONFIG_H
#define CONFIG_H

// Estrutura para armazenar configurações
typedef struct {
    // Unidades de tempo
    int time_unit_ms;
    
    // Limites gerais
    int max_emergency_patients;
    int max_appointments;
    int max_surgeries_pending;
    
    // Centro de Triagem
    int triage_simultaneous_patients;
    int triage_critical_stability;
    int triage_emergency_duration;
    int triage_appointment_duration;
    
    // Blocos Operatórios
    int bo1_min_duration;
    int bo1_max_duration;
    int bo2_min_duration;
    int bo2_max_duration;
    int bo3_min_duration;
    int bo3_max_duration;
    int cleanup_min_time;
    int cleanup_max_time;
    int max_medical_teams;
    
    // Farmácia
    int pharmacy_preparation_time_min;
    int pharmacy_preparation_time_max;
    int auto_restock_enabled;
    int restock_quantity_multiplier;
    
    // Laboratórios
    int lab1_test_min_duration;
    int lab1_test_max_duration;
    int lab2_test_min_duration;
    int lab2_test_max_duration;
    int max_simultaneous_tests_lab1;
    int max_simultaneous_tests_lab2;
} Config;

// Função para carregar configurações
int load_config(const char *filename, Config *config);

#endif // CONFIG_H
