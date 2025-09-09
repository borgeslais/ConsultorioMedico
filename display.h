#ifndef _DISPLAY_H
#define _DISPLAY_H 
#include "consultation.h"

void clear_input_buffer();
void display_menu();
void display_new_consultation(List *lista);
void display_list_consultations_meds(List *lista);
void display_list_consultations(List *lista);
void display_add_medication(List *lista);
void display_remove_consultation(List *lista);
void display_remove_medication(List *lista);
void display_statistics(List *lista);

#endif