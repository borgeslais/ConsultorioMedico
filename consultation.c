#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultation.h"

// Cria lista com nó auxiliar
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

  // Testa validade de todos os dados da consulta, se não providenciados, retorna erro
  if (patient == NULL || strlen(patient) == 0 || description == NULL || strlen(description) == 0 || time == NULL || strlen(time) == 0) {
    printf("Erro: todos os campos devem ser preenchidos!\n");
    return 0;
  }

  // Copia todos os dados da consulta
  strcpy(new_consult->patient_name, patient);
  strcpy(new_consult->description, description);
  strcpy(new_consult->consultation_time, time);

  // Inicializando lista de medicamentos como vazia
  new_consult->medications = NULL;
  new_consult->next = NULL;
  new_consult->prev = li->tail;

  // Valida e adiciona medicamento
  if (med_name != NULL && strlen(med_name) > 0) {
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
  if (li == NULL || li->head == NULL || consult_pos < 0 || consult_pos >= li->size)
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

int remove_consultation(List *li, int pos) {
  // Verificação inicial de parâmetros inválidos
  if (li == NULL || li->head == NULL || pos < 0 || pos >= li->size)
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
  // Verificação inicial de parâmetros inválidos
  if (li == NULL || li->head == NULL || consult_pos < 0 || consult_pos >= li->size)
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
int get_consultation_info(List *li, int pos, char patient[MAX], char description[MAX], char time[MAX]){
  if (li == NULL || li->head == NULL || pos < 0 || pos >= li->size) {
    strcpy(patient, "ERRO: Lista inválida.");
    strcpy(description, "ERRO: Lista inválida.");
    strcpy(time, "ERRO: Lista inválida.");
    return 0;
  }

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
  if (li == NULL || li->head == NULL || consult_pos < 0 || consult_pos >= li->size)
  {
    strcpy(name, "ERRO: Lista inválida.");
    strcpy(use, "ERRO: Lista inválida.");
    strcpy(time, "ERRO: Lista inválida.");
    return 0;
  }

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

  if (current_med != NULL) {
    strcpy(name, current_med->name);
    strcpy(use, current_med->use);
    strcpy(time, current_med->time);
    return 1;
  } else {
    strcpy(name, "ERRO: Medicamento não encontrado.");
    strcpy(use, "ERRO: Medicamento não encontrado.");
    strcpy(time, "ERRO: Medicamento não encontrado.");
    return 0;
  }
}

int get_consultation_med_count(List *li, int pos) {
  if (li == NULL || li->head == NULL || pos < 0 || pos >= li->size)
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

int load_consultations(List *li, const char *consultations_file) {
    if (li == NULL || consultations_file == NULL) return 0;

    FILE *file = fopen(consultations_file, "r");
    if (file == NULL) {
        printf("Arquivo %s não encontrado!\n", consultations_file);
        return 0;
    }

    char line[500];
    int consult_count = 0;
    int current_consult_index = -1;
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
            continue;
        }

        // Lê os três campos
        char field1[MAX], field2[MAX], field3[MAX];
        if (sscanf(trimmed, "%[^;];%[^;];%[^\n]", field1, field2, field3) == 3) {
          if (expect_consultation) {
            // É uma nova consulta
            if (insert_consultation(li, field1, field2, field3, "", "", "")) {
              current_consult_index = consult_count;
              consult_count++;
              expect_consultation = 0; // Agora espera medicamentos
            }
          } else {
            // É um medicamento para a consulta atual
            if (current_consult_index >= 0) {
              insert_medication(li, current_consult_index, field1, field2, field3);
            }
          }
        }
    }

    fclose(file);
    printf("Carregadas %d consultas do arquivo %s\n", consult_count, consultations_file);
    return consult_count;
}