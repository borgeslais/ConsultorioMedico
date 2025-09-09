#include <stdio.h>
#include <string.h>
#include "display.h"

void clear_input_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void display_menu() {
  printf("\n=== SISTEMA DE CONSULTAS MÉDICAS ===\n");
  printf("1. Nova Consulta\n");
  printf("2. Adicionar Medicamento a Consulta\n");
  printf("3. Listar Todas as Consultas\n");
  printf("4. Remover Consulta\n");
  printf("5. Remover Medicamento\n");
  printf("6. Estatísticas\n");
  printf("0. Sair\n");
  printf("Escolha uma opção: ");
}

void display_new_consultation(List *lista) {
  char patient[MAX], desc[MAX], time[MAX], med_name[MAX], med_use[MAX], med_time[MAX];
  printf("\n--- NOVA CONSULTA ---\n");

  printf("Nome do paciente: ");
  fgets(patient, MAX, stdin);
  patient[strcspn(patient, "\n")] = '\0';

  printf("Descrição da consulta: ");
  fgets(desc, MAX, stdin);
  desc[strcspn(desc, "\n")] = '\0';

  printf("Horário da consulta: ");
  fgets(time, MAX, stdin);
  time[strcspn(time, "\n")] = '\0';

  printf("Adicionar medicamento? (s/n): ");
  char opcao = getchar();
  clear_input_buffer();

  if (opcao == 's' || opcao == 'S') {
    printf("Nome do medicamento: ");
    fgets(med_name, MAX, stdin);
    med_name[strcspn(med_name, "\n")] = '\0';

    printf("Quantidade do medicamento: ");
    fgets(med_use, MAX, stdin);
    med_use[strcspn(med_use, "\n")] = '\0';

    printf("Horário do medicamento: ");
    fgets(med_time, MAX, stdin);
    med_time[strcspn(med_time, "\n")] = '\0';

    if (insert_consultation(lista, patient, desc, time, med_name, med_use, med_time)){
      printf("Consulta criada com sucesso!\n");
    } else {
      printf("Erro ao criar consulta!\n");
    }
  } else {
    char empty[100] = "";
    if (insert_consultation(lista, patient, desc, time, empty, empty, empty)) {
      printf("Consulta criada com sucesso!\n");
    } else {
      printf("Erro ao criar consulta!\n");
    }
  }
}

void display_list_consultations_meds(List *lista) {
  printf("\n--- CONSULTAS DISPONÍVEIS ---\n");

  int size = get_list_size(lista);
  if (size == 0) {
    printf("Nenhuma consulta cadastrada.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    char patient[MAX], desc[MAX], time[MAX];
    get_consultation_info(lista, i, patient, desc, time);
    printf("%d. %s - %s (%s)\n", i, patient, desc, time);
  }
}

void display_list_consultations(List *lista) {
  printf("\n--- LISTA COMPLETA DE CONSULTAS ---\n");

  int size = get_list_size(lista);
  if (size == 0) {
    printf("Nenhuma consulta cadastrada.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    char patient[MAX], desc[MAX], time[MAX];
    if (get_consultation_info(lista, i, patient, desc, time) == 0) {
      printf("\n--- Consulta %d (Erro) ---\n", i + 1);
      printf("Erro ao carregar consulta %d\n", i + 1);
      continue;
    }

    printf("\n--- Consulta %d ---\n", i + 1);
    printf("Paciente: %s\n", patient);
    printf("Descrição: %s\n", desc);
    printf("Horário: %s\n", time);

    int med_count = get_consultation_med_count(lista, i);
    if (med_count == 0) {
      printf("Medicamentos: Nenhum\n");
    } else {
      printf("Medicamentos:\n");
      for (int j = 0; j < med_count; j++) {
        char med_name[MAX], med_use[MAX], med_time[MAX];
        if (get_medication_info(lista, i, j, med_name, med_use, med_time)) {
          printf("  %d. %s - %s (%s)\n", j + 1, med_name, med_use, med_time);
        } else {
          printf("  %d. %s\n", j + 1, med_name);
        }
      }
    }
  }
}

void display_add_medication(List *lista) {
  int consult_pos;
  char name[MAX], use[MAX], time[MAX];

  printf("\n--- ADICIONAR MEDICAMENTO ---\n");
  display_list_consultations_meds(lista);

  printf("\nDigite o número da consulta: ");
  if (scanf("%d", &consult_pos) != 1)
  {
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }
  clear_input_buffer();

  printf("Nome do medicamento: ");
  fgets(name, MAX, stdin);
  name[strcspn(name, "\n")] = '\0';

  printf("Quantidade do medicamento: ");
  fgets(use, MAX, stdin);
  use[strcspn(use, "\n")] = '\0';

  printf("Horário do medicamento: ");
  fgets(time, MAX, stdin);
  time[strcspn(time, "\n")] = '\0';

  if (insert_medication(lista, consult_pos, name, use, time)) {
    printf("Medicamento adicionado com sucesso!\n");
  } else {
    printf("Erro ao adicionar medicamento!\n");
  }
}

void display_remove_consultation(List *lista) {
  int pos;
  printf("\n--- REMOVER CONSULTA ---\n");
  display_list_consultations_meds(lista);
  printf("\nDigite o número da consulta a remover: ");
  // verifica se realmente leu um número
  if (scanf("%d", &pos) != 1) { 
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }
  // Limpa caracteres extras da entrada
  clear_input_buffer(); 

  if (pos < 0 || pos >= get_list_size(lista)) {
    printf("Consulta inválida!\n");
    return;
  }

  if (remove_consultation(lista, pos)) {
    printf("Consulta removida com sucesso!\n");
  } else {
    printf("Erro ao remover consulta!\n");
  }
}

void display_remove_medication(List *lista) {
  int consult_pos, med_pos;

  printf("\n--- REMOVER MEDICAMENTO ---\n");
  display_list_consultations_meds(lista);

  printf("\nDigite o número da consulta: ");
  if (scanf("%d", &consult_pos) != 1) {
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }

  // Mostra medicamentos desta consulta
  int med_count = get_consultation_med_count(lista, consult_pos);
  if (med_count == 0) {
    printf("Esta consulta não tem medicamentos.\n");
    clear_input_buffer();
    return;
  }

  printf("Medicamentos da consulta %d:\n", consult_pos);
  for (int i = 0; i < med_count; i++) {
    char name[MAX], use[MAX], time[MAX];
    get_medication_info(lista, consult_pos, i, name, use, time);
    printf("%d. %s\n", i, name);
  }

  printf("Digite o número do medicamento a remover: ");
  if (scanf("%d", &med_pos) != 1) {
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }
  clear_input_buffer();

  if (remove_medication(lista, consult_pos, med_pos)) {
    printf("Medicamento removido com sucesso!\n");
  } else {
    printf("Erro ao remover medicamento!\n");
  }
}

void display_statistics(List *lista) {
  printf("\n--- ESTATÍSTICAS ---\n");
  printf("Total de consultas: %d\n", get_list_size(lista));

  int total_meds = 0;
  int max_meds = 0;
  int consult_com_mais_meds = -1;

  for (int i = 0; i < get_list_size(lista); i++) {
    int med_count = get_consultation_med_count(lista, i);
    total_meds += med_count;

    if (med_count > max_meds) {
      max_meds = med_count;
      consult_com_mais_meds = i;
    }
  }

  printf("Total de medicamentos prescritos: %d\n", total_meds);

  if (consult_com_mais_meds != -1) {
    char patient[MAX], desc[MAX], time[MAX];
    get_consultation_info(lista, consult_com_mais_meds, patient, desc, time);
    printf("Consulta com mais medicamentos: %s (%d medicamentos)\n", patient, max_meds);
  }
}