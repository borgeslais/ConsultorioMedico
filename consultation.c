#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultation.h"

// Cria lista
List *create_list() {
  List *li = (List *)malloc(sizeof(List));
  if (li != NULL) {
    li->head = NULL;
    li->tail = NULL;
    li->size = 0;
  }
  return li;
}

// Libera lista
int free_list(List *li) {
  if (li == NULL)
    return 0;
  Consultation *current = li->head;
  while (current != NULL) {
    Consultation *next = current->next;
    Medication *current_med = current->medications;
    while (current_med != NULL) {
      Medication *next_med = current_med->next;
      free(current_med);
      current_med = next_med;
    }
    free(current);
    current = next;
  }
  free(li);
  return 1;
}

// Insere consulta
int insert_consultation(List *li, const char *patient, const char *description, const char *time, const char *med_name, const char *med_use, const char *med_time) {
  if (li == NULL)
    return 0;

  Consultation *new_consult = (Consultation *)malloc(sizeof(Consultation));
  if (new_consult == NULL)
    return 0;

  // Copia todos os dados da consulta
  strcpy(new_consult->patient_name, patient);
  strcpy(new_consult->description, description);
  strcpy(new_consult->consultation_time, time);

  // Inicializando lista de medicamentos como vazia
  new_consult->medications = NULL;
  new_consult->next = NULL;
  new_consult->prev = li->tail;

  // Valida e adiciona medicamento
  if (med_name != NULL) { 
    Medication *new_med = (Medication *)malloc(sizeof(Medication));
    if (new_med == NULL) {
      free(new_consult);
      return 0;
    }
    // Copia todos os dados da medicação
    strcpy(new_med->name, med_name);
    strcpy(new_med->use, med_use);
    strcpy(new_med->time, med_time);
    new_med->next = NULL;
    new_med->prev = NULL;

    new_consult->medications = new_med;
  }

  // Adiciona nova consulta à lista
  if (li->head == NULL) {
    li->head = new_consult;
    li->tail = new_consult;
  } else {
    li->tail->next = new_consult;
    li->tail = new_consult;
  }
  li->size++;
  return 1;
}

// Insere medicamento
int insert_medication(List *li, int consult_pos, const char *name, const char *use, const char *time) {
  if (li == NULL)
    return 0;

  Consultation *current_consult = li->head;
  int count = 0;
  while (current_consult != NULL && count < consult_pos) {
    current_consult = current_consult->next;
    count++;
  }

  if (current_consult == NULL)
    return 0;

  Medication *new_med = (Medication *)malloc(sizeof(Medication));
  if (new_med == NULL)
    return 0;

  strcpy(new_med->name, name);
  strcpy(new_med->use, use);
  strcpy(new_med->time, time);
  new_med->next = NULL;
  new_med->prev = NULL;

  if (current_consult->medications == NULL) {
    current_consult->medications = new_med;
  } else {
    Medication *current_med = current_consult->medications;
    while (current_med->next != NULL) {
      current_med = current_med->next;
    }
    current_med->next = new_med;
    new_med->prev = current_med;
  }
  return 1;
}

// Faz busca por paciente e retorna lista
List* search_by_patient(List *li, const char *patient_name) {
    if (li == NULL || patient_name == NULL)
        return NULL;

    List *results = create_list();
    if (results == NULL)
        return NULL;

    Consultation *current = li->head;
    int found_count = 0;
    while (current != NULL) {
        if (strcasecmp(current->patient_name, patient_name) == 0) {
          insert_consultation_ptr(results, current);
          found_count++;
        }
        current = current->next;
    }
    return results;
}

int remove_consultation(List *li, int pos) {
  if (li == NULL)
    return 0;

  // Navega até a consulta desejada
  Consultation *to_remove = li->head;
  for (int i = 0; i < pos && to_remove != NULL; i++) {
    to_remove = to_remove->next;
  }

  if (to_remove == NULL)
    return 0;

  // Atualiza os ponteiros da lde
  if (to_remove->prev != NULL) {
    to_remove->prev->next = to_remove->next;
  } else {
    li->head = to_remove->next;
  }

  if (to_remove->next != NULL) {
    to_remove->next->prev = to_remove->prev;
  } else {
    li->tail = to_remove->prev;
  }

  // Libera todos os medicamentos da consulta
  Medication *current_med = to_remove->medications;
  while (current_med != NULL) {
    Medication *next_med = current_med->next;
    free(current_med);
    current_med = next_med;
  }

  // Libera a consulta e atualiza o tamanho da lista
  free(to_remove);
  li->size--;
  return 1;
}

int remove_medication(List *li, int consult_pos, int med_pos) {
  if (li == NULL)
    return 0;

  // Navega até consulta desejada
  Consultation *current_consult = li->head;
  for (int i = 0; i < consult_pos; i++) {
    if (current_consult == NULL)
      return 0;
    current_consult = current_consult->next;
  }

  if (current_consult == NULL)
    return 0;

  // Verifica se a lista de medicamentos está vazia
  if (current_consult->medications == NULL) {
    return 0;
  }

  // Caso especial: remover o primeiro medicamento
  if (med_pos == 0) {
    Medication *to_remove = current_consult->medications;
    current_consult->medications = to_remove->next;

    if (to_remove->next != NULL) {
      to_remove->next->prev = NULL;
    }

    free(to_remove);
    return 1;
  }

  // Navega até o medicamento desejado
  Medication *current_med = current_consult->medications;
  for (int i = 0; i < med_pos; i++) {
    if (current_med == NULL)
      return 0;
    current_med = current_med->next;
  }

  if (current_med == NULL)
    return 0;

  // Remove o medicamento
  current_med->prev->next = current_med->next;
  if (current_med->next != NULL)
    current_med->next->prev = current_med->prev;

  free(current_med);
  return 1;
}

// Mostra informações da consulta
int get_consultation_info(List *li, int pos, char patient[MAX], char description[MAX], char time[MAX]) {
  if (li == NULL)
    return 0;

  Consultation *current = li->head;  
  int count = 0;
  while (count < pos) {
    current = current->next;
    count++;
  }

  strcpy(patient, current->patient_name);
  strcpy(description, current->description);
  strcpy(time, current->consultation_time);
  return 1;
}

int get_list_size(List *li) {
  if (li == NULL)
    return 0;
  return li->size;
}

int get_medication_info(List *li, int consult_pos, int med_pos, char name[MAX], char use[MAX], char time[MAX]) {
  if (li == NULL)
    return 0;

  Consultation *current_consult = li->head;
  int count = 0;

  while (count < consult_pos) {
    current_consult = current_consult->next;
    count++;
  }

  Medication *current_med = current_consult->medications;
  count = 0;
  while (current_med != NULL && count < med_pos) {
    current_med = current_med->next;
    count++;
  }

  if (current_med == NULL) {
    return 0;
  } else {
    strcpy(name, current_med->name);
    strcpy(use, current_med->use);
    strcpy(time, current_med->time);
    return 1;
  }
}

int get_consultation_med_count(List *li, int pos) {
  if (li == NULL)
    return 0;

  Consultation *current = li->head;
  for (int i = 0; i < pos && current != NULL; i++) {
    current = current->next;
  }

  if (current == NULL)
    return 0;

  int med_count = 0;
  Medication *current_med = current->medications;
  while (current_med != NULL) {
    med_count++;
    current_med = current_med->next;
  }

  return med_count;
}

char* get_file_name() {
    char *file_name = malloc(25 * sizeof(char));
    if (!file_name) return NULL;
    
    printf("Digite o nome do arquivo que será carregado:\n");
    scanf("%24s", file_name);
    return file_name;
}

int load_consultations(List *li, const char *consultations_file) {
    if (li == NULL || consultations_file == NULL) return 0;

    FILE *file = fopen(consultations_file, "r");
    if (file == NULL) {
        printf("Arquivo %s não encontrado!\n", consultations_file);
        return 0;
    }

    char line[500];
    int consult_count = 0;
    Consultation *current_consultation = NULL; 
    int expect_consultation = 1; // 1 = espera consulta, 0 = espera medicamentos

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove quebra de linha e espaços
        line[strcspn(line, "\n")] = '\0';
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        int len = strlen(trimmed);
        
        // Linha em branco = nova consulta
        if (len == 0) {
            expect_consultation = 1;
            // Reseta
            current_consultation = NULL;
            continue;
        }

        // Lê os três campos: nome do paciente, descrição e data
        char field1[MAX], field2[MAX], field3[MAX];
        if (sscanf(trimmed, "%[^;];%[^;];%[^\n]", field1, field2, field3) == 3) {
          if (expect_consultation) {
            // Adiciona nova consulta
            if (insert_consultation(li, field1, field2, field3, NULL, NULL, NULL)) {
              current_consultation = li->tail; 
              consult_count++;
              expect_consultation = 0; // Agora espera medicamentos
            }
          } else {
            // É um medicamento para a consulta atual
            if (current_consultation != NULL) {
              if (strlen(field1) > 0 && strlen(field2) > 0 && strlen(field3) > 0){
                // Insere nome do medicamento, dosagem e hora
                insert_medication_to_consultation(current_consultation, field1, field2, field3);
              }
            }
          }
        }
    }

    fclose(file);
    printf("Carregadas %d consultas do arquivo %s\n", consult_count, consultations_file);
    return consult_count;
}

// Função auxiliar
int insert_consultation_ptr(List *li, Consultation *consult_ptr) {
    if (li == NULL || consult_ptr == NULL) return 0;
    
    Consultation *new_node = (Consultation*)malloc(sizeof(Consultation));
    if (new_node == NULL) return 0;
    
    strncpy(new_node->patient_name, consult_ptr->patient_name, MAX);
    strncpy(new_node->description, consult_ptr->description, MAX);
    strncpy(new_node->consultation_time, consult_ptr->consultation_time, MAX);
    
    new_node->medications = consult_ptr->medications;
    
    new_node->next = NULL;
    new_node->prev = NULL;
    
    if (li->tail == NULL) {
        li->head = new_node;
        li->tail = new_node;
    } else {
        li->tail->next = new_node;
        new_node->prev = li->tail;
        li->tail = new_node;
    }
    
    li->size++;
    return 1;
}

// Função auxiliar
int insert_medication_to_consultation(Consultation *consult, const char *name, const char *use, const char *time) {
    if (consult == NULL) return 0;
    
    Medication *new_med = (Medication*)malloc(sizeof(Medication));
    if (new_med == NULL) return 0;
    
    strncpy(new_med->name, name, MAX);
    strncpy(new_med->use, use, MAX);
    strncpy(new_med->time, time, MAX);
    new_med->next = NULL;
    new_med->prev = NULL;
    
    // Adiciona no final da lista de medicamentos
    if (consult->medications == NULL) {
        consult->medications = new_med;
    } else {
        Medication *last = consult->medications;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_med;
        new_med->prev = last;
    }
    
    return 1;
}