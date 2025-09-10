#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultation.h"
#include "display.h"

int main()
{
  List *li = create_list();
  if (li == NULL) {
    printf("Erro ao inicializar sistema!\n");
    return 1;
  }
  
  int option;

  printf("Bem-vindo ao Sistema de Gerenciamento de Consultas!\n");
  do {
    display_menu();
    if (scanf("%d", &option) != 1) {
      printf("Entrada inválida!\n");
      clear_input_buffer();
      continue;
    }
    clear_input_buffer();
    switch (option) {
    case 1:
      display_new_consultation(li);
      break;
    case 2:
      display_add_medication(li);
      break;
    case 3:
      display_list_consultations(li);
      break;
    case 4:
      display_remove_consultation(li);
      break;
    case 5:
      display_remove_medication(li);
      break;
    case 6:
      display_statistics(li);
      break;
    case 7:
      char *file_name = get_file_name();
      if (file_name) {
        load_consultations(li, file_name);
        free(file_name);
      }
      break;
    case 8:
      char search_name[MAX];
      printf("Digite o nome do paciente para buscar: ");
      if (fgets(search_name, MAX, stdin) == NULL) {
        printf("Erro na leitura do nome!\n");
        break;
      }
      search_name[strcspn(search_name, "\n")] = '\0';
      if (strlen(search_name) == 0) {
        printf("Nome não pode estar vazio!\n");
        break;
    }
      search_and_display_by_patient(li, search_name);
      break;
    case 0:
      printf("Saindo do sistema...\n");
      printf("Obrigado por usar nosso sistema!\n");
      break;
    default:
      printf("Opção inválida!\n");
    }
  } while (option != 0);

  free_list(li);
  return 0;
}