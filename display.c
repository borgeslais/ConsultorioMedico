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
  printf("7. Carregar um arquivo\n");
  printf("8. Fazer busca por nome\n");
  printf("0. Sair\n");
  printf("Escolha uma opção: ");
}

void display_new_consultation(List *li) {
  char patient[MAX], desc[MAX], time[MAX], med_name[MAX], med_use[MAX], med_time[MAX];
  printf("\n--- NOVA CONSULTA ---\n");

  do {
    printf("Nome do paciente: ");
    if (fgets(patient, MAX, stdin) == NULL) {
      printf("Erro na leitura!\n");
      return;
    }
    patient[strcspn(patient, "\n")] = '\0';

    if (strlen(patient) == 0)
      printf("É necessário informar nome do paciente!\n");
  } while (strlen(patient) == 0);

  do {
    printf("Descrição da consulta: ");
    if (fgets(desc, MAX, stdin) == NULL) {
      printf("Erro na leitura!\n");
      return;
    }
    desc[strcspn(desc, "\n")] = '\0';

    if (strlen(desc) == 0)
      printf("É necessário informar a descrição da consulta!\n");
  } while (strlen(desc) == 0);

  do {
    printf("Horário da consulta (formato: DD-MM-AAAA HH:MM): ");
    if (fgets(time, MAX, stdin) == NULL) {
      printf("Erro na leitura!\n");
      return;
    }
    time[strcspn(time, "\n")] = '\0';

    if (strlen(time) == 0) {
      printf("É necessário informar o horário da consulta!\n");
    }
  } while (strlen(time) == 0);

  printf("Adicionar medicamento? (s/n): ");
  char opcao = getchar();
  clear_input_buffer();

  if (opcao == 's' || opcao == 'S') {
    do {
      printf("Nome do medicamento: ");
      if (fgets(med_name, MAX, stdin) == NULL) {
        printf("Erro na leitura!\n");
        return;
      }
      med_name[strcspn(med_name, "\n")] = '\0';

      if (strlen(med_name) == 0)
        printf("É necessário informar o nome do medicamento!\n");
    } while (strlen(med_name) == 0);

    do {
      printf("Dosagem do medicamento: ");
      if (fgets(med_use, MAX, stdin) == NULL){
        printf("Erro na leitura!\n");
        return;
      }
      med_use[strcspn(med_use, "\n")] = '\0';

      if (strlen(med_use) == 0)
        printf("É necessário informar a dosagem!\n");
    } while (strlen(med_use) == 0);

    do {
      printf("Horário do medicamento: ");
      if (fgets(med_time, MAX, stdin) == NULL){
        printf("Erro na leitura!\n");
        return;
      }
      med_time[strcspn(med_time, "\n")] = '\0';

      if (strlen(med_time) == 0)
        printf("É necessário informar o horário do medicamento!\n");
    } while (strlen(med_time) == 0);

    if (insert_consultation(li, patient, desc, time, med_name, med_use, med_time)) {
      printf("Consulta criada com sucesso!\n");
    } else {
      printf("Erro ao criar consulta!\n");
    }
  } else {
    char empty[100] = "";
    if (insert_consultation(li, patient, desc, time, empty, empty, empty)) {
      printf("Consulta criada com sucesso!\n");
    } else {
      printf("Erro ao criar consulta!\n");
    }
  }
}

void display_list_consultations_meds(List *li) {
  printf("\n--- CONSULTAS DISPONÍVEIS ---\n");

  int size = get_list_size(li);
  if (size == 0) {
    printf("Nenhuma consulta cadastrada.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    char patient[MAX], desc[MAX], time[MAX];
    get_consultation_info(li, i, patient, desc, time);
    printf("%d. %s - %s (%s)\n", i, patient, desc, time);
  }
}

void display_list_consultations(List *li) {
  printf("\n--- LISTA COMPLETA DE CONSULTAS ---\n");

  int size = get_list_size(li);
  if (size == 0) {
    printf("Nenhuma consulta cadastrada.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    char patient[MAX], desc[MAX], time[MAX];
    if (get_consultation_info(li, i, patient, desc, time) == 0) {
      printf("\n--- Consulta %d (Erro) ---\n", i + 1);
      printf("Erro ao carregar consulta %d\n", i + 1);
      continue;
    }

    printf("\n--- Consulta %d ---\n", i + 1);
    printf("Paciente: %s\n", patient);
    printf("Descrição: %s\n", desc);
    printf("Horário: %s\n", time);

    int med_count = get_consultation_med_count(li, i);
    if (med_count == 0) {
      printf("Medicamentos: Nenhum\n");
    } else {
      printf("Medicamentos:\n");
      for (int j = 0; j < med_count; j++) {
        char med_name[MAX], med_use[MAX], med_time[MAX];
        if (get_medication_info(li, i, j, med_name, med_use, med_time)) {
          printf("  %d. %s - %s (%s)\n", j + 1, med_name, med_use, med_time);
        } else {
          printf("  %d. %s\n", j + 1, med_name);
        }
      }
    }
  }
}

void display_add_medication(List *li) {
  int consult_pos;
  char name[MAX], use[MAX], time[MAX];

  printf("\n--- ADICIONAR MEDICAMENTO ---\n");
  display_list_consultations_meds(li);

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

  if (insert_medication(li, consult_pos, name, use, time)) {
    printf("Medicamento adicionado com sucesso!\n");
  } else {
    printf("Erro ao adicionar medicamento!\n");
  }
}

void display_remove_consultation(List *li) {
  int pos;
  printf("\n--- REMOVER CONSULTA ---\n");
  display_list_consultations_meds(li);
  printf("\nDigite o número da consulta a remover: ");
  // verifica se realmente leu um número
  if (scanf("%d", &pos) != 1) { 
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }
  // Limpa caracteres extras da entrada
  clear_input_buffer(); 

  if (pos < 0 || pos >= get_list_size(li)) {
    printf("Consulta inválida!\n");
    return;
  }

  if (remove_consultation(li, pos)) {
    printf("Consulta removida com sucesso!\n");
  } else {
    printf("Erro ao remover consulta!\n");
  }
}

void display_remove_medication(List *li) {
  int consult_pos, med_pos;

  printf("\n--- REMOVER MEDICAMENTO ---\n");
  display_list_consultations_meds(li);

  printf("\nDigite o número da consulta: ");
  if (scanf("%d", &consult_pos) != 1) {
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }

  // Mostra medicamentos desta consulta
  int med_count = get_consultation_med_count(li, consult_pos);
  if (med_count == 0) {
    printf("Esta consulta não tem medicamentos.\n");
    clear_input_buffer();
    return;
  }

  printf("Medicamentos da consulta %d:\n", consult_pos);
  for (int i = 0; i < med_count; i++) {
    char name[MAX], use[MAX], time[MAX];
    get_medication_info(li, consult_pos, i, name, use, time);
    printf("%d. %s\n", i, name);
  }

  printf("Digite o número do medicamento a remover: ");
  if (scanf("%d", &med_pos) != 1) {
    printf("Entrada inválida!\n");
    clear_input_buffer();
    return;
  }
  clear_input_buffer();

  if (remove_medication(li, consult_pos, med_pos)) {
    printf("Medicamento removido com sucesso!\n");
  } else {
    printf("Erro ao remover medicamento!\n");
  }
}

void display_statistics(List *li) {
  printf("\n--- ESTATÍSTICAS ---\n");
  printf("Total de consultas: %d\n", get_list_size(li));

  int total_meds = 0;
  int max_meds = 0;
  int consult_com_mais_meds = -1;

  for (int i = 0; i < get_list_size(li); i++) {
    int med_count = get_consultation_med_count(li, i);
    total_meds += med_count;

    if (med_count > max_meds) {
      max_meds = med_count;
      consult_com_mais_meds = i;
    }
  }

  printf("Total de medicamentos prescritos: %d\n", total_meds);

  if (consult_com_mais_meds != -1) {
    char patient[MAX], desc[MAX], time[MAX];
    get_consultation_info(li, consult_com_mais_meds, patient, desc, time);
    printf("Consulta com mais medicamentos: %s (%d medicamentos)\n", patient, max_meds);
  }
}

void search_and_display_by_patient(List *li, const char *patient_name) {
    if (li == NULL || patient_name == NULL) {
        printf("Parâmetros inválidos!\n");
        return;
    }

    if (li->head == NULL) {
        printf("Nenhuma consulta encontrada para: '%s'\n", patient_name);
        return;
    }

    Consultation *current = li->head;
    int found_count = 0;

    printf("\n=== RESULTADOS DA BUSCA PARA: '%s' ===\n", patient_name);

    while (current != NULL) {
        if (strcasecmp(current->patient_name, patient_name) == 0) {
            found_count++;
            printf("\n--- Resultado %d ---\n", found_count);
            printf("Paciente: %s\n", current->patient_name);
            printf("Descrição: %s\n", current->description);
            printf("Horário: %s\n", current->consultation_time);
            
            // Display medications from the consultation's own medication list
            if (current->medications != NULL) {
                printf("Medicamentos:\n");
                Medication *med = current->medications;
                int med_count = 1;
                while (med != NULL) {
                    printf("  %d. %s - %s (%s)\n", 
                           med_count, med->name, med->use, med->time);
                    med = med->next;
                    med_count++;
                }
            } else {
                printf("Medicamentos: Nenhum\n");
            }
            printf("----------------------------\n");
        }
        current = current->next;
    }

    if (found_count == 0) {
        printf("Nenhuma consulta encontrada para: '%s'\n", patient_name);
    } else {
        printf("\nTotal encontrado: %d consulta(s)\n", found_count);
    }
    
    printf("=== FIM DOS RESULTADOS ===\n");
}