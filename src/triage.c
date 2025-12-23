// Triagem: macro de debug com indentação e sintaxe corrigidas
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
	fprintf(stderr, "[DEBUG][%s:%d:%s] " fmt "\n", \
			__FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

// Exemplo de uso (comentado para evitar erros de compilação):
// DEBUG_PRINT("Paciente %s entrou na fila", patient_id);
// DEBUG_PRINT("Stock ANALG_A: %d unidades", stock);

#include "../include/hospital.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static volatile sig_atomic_t running = 1;
static TriageCenter triage;

/* ================= SIGNAL ================= */

void sigterm_handler(int sig) {
    (void)sig;
    running = 0;
}

/* ================= INIT ================= */

void init_triage_center() {
    pthread_mutex_init(&triage.mutex, NULL);
    pthread_cond_init(&triage.cond, NULL);

    triage.emergency_count = 0;
    triage.appointment_count = 0;
    triage.active_patients = 0;
    triage.total_triaged = 0;
}

/* ================= THREADS ================= */

void* stability_monitor(void* arg) {
    (void)arg;

    while (running) {
        pthread_mutex_lock(&triage.mutex);

        for (int i = 0; i < triage.emergency_count; i++) {
            triage.emergency_queue[i].stability--;

            if (triage.emergency_queue[i].stability <=
                triage.emergency_queue[i].critical_threshold) {
                triage.emergency_queue[i].is_critical = 1;
            }
        }

        pthread_mutex_unlock(&triage.mutex);
        sleep(1); // 1 time unit
    }

    pthread_exit(NULL);
}

void* emergency_manager(void* arg) {
    (void)arg;

    while (running) {
        pthread_mutex_lock(&triage.mutex);

        while (triage.emergency_count == 0 && running) {
            pthread_cond_wait(&triage.cond, &triage.mutex);
        }

        if (!running) {
            pthread_mutex_unlock(&triage.mutex);
            break;
        }

        Patient p = triage.emergency_queue[0];

        // shift da fila
        memmove(&triage.emergency_queue[0],
                &triage.emergency_queue[1],
                sizeof(Patient) * (triage.emergency_count - 1));

        triage.emergency_count--;
        triage.active_patients++;

        pthread_mutex_unlock(&triage.mutex);

        printf("[TRIAGE] A atender emergência %s\n", p.patient_id);
        sleep(3); // tempo de atendimento

        pthread_mutex_lock(&triage.mutex);
        triage.active_patients--;
        triage.total_triaged++;
        pthread_mutex_unlock(&triage.mutex);
    }

    pthread_exit(NULL);
}

/* ================= MAIN ================= */

int main() {
    pthread_t t_emerg, t_stability;

    printf("[TRIAGE] Centro de Triagem iniciado (PID=%d)\n", getpid());

    signal(SIGTERM, sigterm_handler);

    init_triage_center();

    pthread_create(&t_emerg, NULL, emergency_manager, NULL);
    pthread_create(&t_stability, NULL, stability_monitor, NULL);

    while (running) {
        sleep(1);
    }

    pthread_join(t_emerg, NULL);
    pthread_join(t_stability, NULL);

    printf("[TRIAGE] Encerrado corretamente\n");
    return 0;
}