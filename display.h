#ifndef _DISPLAY_H
#define _DISPLAY_H 
#include "consultation.h"

void clear_input_buffer();
void display_menu();
void display_new_consultation(List *li);
void display_list_consultations_meds(List *li);
void display_list_consultations(List *li);
void display_add_medication(List *li);
void display_remove_consultation(List *li);
void display_remove_medication(List *li);
void search_and_display_by_patient(List *li, const char *patient_name);
void display_statistics(List *li);

#endif