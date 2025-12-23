#define _POSIX_C_SOURCE 200809L

// ================= DEBUG MACRO =================
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "[DEBUG][%s:%d:%s] " fmt "\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

// ================= INCLUDES =================
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "../include/hospital.h"

// ================= DEFINES =================
#define INPUT_PIPE "input_pipe"

// ================= GLOBAL =================
volatile sig_atomic_t shutdown_flag = 0;

// ================= SIGNAL HANDLER =================
void sigint_handler(int sig) {
    (void)sig;
    shutdown_flag = 1;
}

// ================= IPC =================
void create_named_pipes(void) {
    if (mkfifo(INPUT_PIPE, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
}

void cleanup_named_pipes(void) {
    unlink(INPUT_PIPE);
}

// ================= MAIN =================
int main(void) {
    pid_t pid_triage, pid_surgery, pid_pharmacy, pid_labs;

    printf("[SYSTEM] A iniciar sistema hospitalar...\n");

    // ----- Signals -----
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // ----- IPC -----
    create_named_pipes();

    // ----- Fork TRIAGE -----
    pid_triage = fork();
    if (pid_triage == 0) {
        execl("./triage", "triage", NULL);
        perror("execl triage");
        exit(EXIT_FAILURE);
    }

    // ----- Fork SURGERY -----
    pid_surgery = fork();
    if (pid_surgery == 0) {
        execl("./surgery", "surgery", NULL);
        perror("execl surgery");
        exit(EXIT_FAILURE);
    }

    // ----- Fork PHARMACY -----
    pid_pharmacy = fork();
    if (pid_pharmacy == 0) {
        execl("./pharmacy", "pharmacy", NULL);
        perror("execl pharmacy");
        exit(EXIT_FAILURE);
    }

    // ----- Fork LABS -----
    pid_labs = fork();
    if (pid_labs == 0) {
        execl("./laboratory", "laboratory", NULL);
        perror("execl laboratory");
        exit(EXIT_FAILURE);
    }

    printf("[SYSTEM] Todos os processos iniciados\n");

    // ----- MAIN LOOP -----
    while (!shutdown_flag) {
        pause();
    }

    // ----- SHUTDOWN -----
    printf("[SYSTEM] Shutdown iniciado...\n");

    kill(pid_triage, SIGTERM);
    kill(pid_surgery, SIGTERM);
    kill(pid_pharmacy, SIGTERM);
    kill(pid_labs, SIGTERM);

    while (wait(NULL) > 0);

    cleanup_named_pipes();

    printf("[SYSTEM] Sistema encerrado corretamente\n");
    return 0;
}