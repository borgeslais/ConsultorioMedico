#ifndef _CONSULTATION_H
#define _CONSULTATION_H
#define MAX 100

// Estrutura de informações para medicação
typedef struct medication {
  char name[MAX];
  char use[MAX];
  char time[MAX];
  struct medication *next;
  struct medication *prev;
} Medication;

// Estrutura de informações para consulta
typedef struct consultation {
  char patient_name[MAX];
  char consultation_time[MAX];
  char description[MAX];
  Medication *medications;
  struct consultation *next;
  struct consultation *prev;
} Consultation;

// Estrutura principal da lista com head e tail
typedef struct list {
  Consultation *head;
  Consultation *tail;
  int size;
} List;

// Funções de criações de lista
List *create_list();
int free_list(List *li);

// Funções de inserção
int insert_consultation(List *li, const char *patient, const char *desc, const char *time, const char *med_name, const char *med_use, const char *med_time);
int insert_medication(List *li, int consult_pos, const char *name, const char *use, const char *time);

// Função de busca pelo nome do paciente
List* search_by_patient(List *li, const char *patient_name);

// Funções de remoção
int remove_consultation(List *li, int pos);
int remove_medication(List *li, int consult_pos, int med_pos);

// Funções para obter dados
int get_consultation_info(List *li, int pos, char patient[MAX], char desc[MAX], char time[MAX]);
int get_medication_info(List *li, int consult_pos, int med_pos, char name[MAX], char use[MAX], char time[MAX]);
int get_list_size(List *li);
int get_consultation_med_count(List *li, int pos);

// Função para obter dados de arquivo .txt
char* get_file_name();
int load_consultations(List *li, const char *consultations_file);

// Função auxiliar que copia dados de uma consulta -- não copia medicamentos
int insert_consultation_ptr(List *li, Consultation *consult_ptr);
int insert_medication_to_consultation(Consultation *consult, const char *name, const char *use, const char *time);
#endif