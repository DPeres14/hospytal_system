#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <pthread.h>

// SHM5: Log Eventos Críticos
typedef struct {
    time_t timestamp;
    char event_type[30];
    char component[20];
    char description[256];
    int severity;           // 1-5
} critical_event_t;

typedef struct {
    critical_event_t events[1000];
    int event_count;
    int current_index;      // Circular buffer
    pthread_mutex_t mutex;
} critical_log_shm_t;

// Níveis de log
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} LogLevel;

// Estrutura de entrada de log
typedef struct {
    time_t timestamp;
    LogLevel level;
    int patient_id;
    char module[32];
    char message[512];
} LogEntry;

// Funções de logging
int init_logger(const char *filename);
void log_message(LogLevel level, int patient_id, const char *module, const char *format, ...);
void log_patient_event(int patient_id, const char *event, const char *details);
void log_system_event(const char *event, const char *details);
int close_logger(void);

#endif // LOG_H
