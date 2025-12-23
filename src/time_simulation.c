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