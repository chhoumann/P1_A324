#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "user_prefs.h"

#define MAX_LINE_LENGTH 32
#define SPACING 75
#define NUMBER_OF_USER_PREFS 8

/* Prototype methods */
void check_setup(void);
void initial_setup(void);
void check_allergy(char *question, int *pref_value);
void read_user_prefs_from_file(FILE *file);
void generate_user_tags(void);

/* The path of the user preferences .txt file */
const char *user_prefs_file_name = "./user_prefs.txt";

/* Runs the initial setup if the setup file is empty or does not exist */
void check_setup(void) {
    FILE *user_prefs_file = fopen(user_prefs_file_name, "r");

    if (user_prefs_file == NULL || user_prefs_file != NULL && is_file_empty(user_prefs_file))
        initial_setup();
    else
        read_user_prefs_from_file(user_prefs_file);
    
    generate_user_tags();
}

/* Given the user preferences file already exists, we read the preferences from there */
void read_user_prefs_from_file(FILE *user_prefs_file) {
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.gluten_allergy);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.lactose_intolerance);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.nut_allergy);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.vegan);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.quick_meals);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.slow_meals);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.high_protein);
    fscanf(user_prefs_file, "%*s %*c %d", &saved_user_prefs.low_fat);

    fclose(user_prefs_file);
}

/* Prompts the user for allergies and calls the save user preferences method */
void initial_setup(void) {
    printf("Velkommen! Foer vi gaar i gang, skal vi lige vide, om du har nogle allergier.\n");
    check_allergy("Har du glutenallergi? (y/n) ", &saved_user_prefs.gluten_allergy);
    check_allergy("Er du laktose intolerant? (y/n) ", &saved_user_prefs.lactose_intolerance);
    check_allergy("Har du noeddeallergi? (y/n) ", &saved_user_prefs.nut_allergy);
    
    save_user_prefs();
}

/* Saves user preferences to the .txt file using the input parameters */
void save_user_prefs(void) {
    FILE *user_prefs_file = fopen(user_prefs_file_name, "w");

    fprintf(user_prefs_file, "gluten_allergy = %d\n", saved_user_prefs.gluten_allergy);
    fprintf(user_prefs_file, "lactose_intolerance = %d\n", saved_user_prefs.lactose_intolerance);
    fprintf(user_prefs_file, "nut_allergy = %d\n", saved_user_prefs.nut_allergy);    
    fprintf(user_prefs_file, "vegan = %d\n", saved_user_prefs.vegan);
    fprintf(user_prefs_file, "quick_meals = %d\n", saved_user_prefs.quick_meals);
    fprintf(user_prefs_file, "slow_meals = %d\n", saved_user_prefs.slow_meals);
    fprintf(user_prefs_file, "high_protein = %d\n", saved_user_prefs.high_protein);
    fprintf(user_prefs_file, "low_fat = %d", saved_user_prefs.low_fat);    

    fclose(user_prefs_file);
}

/* Scans user's yes/no input in terminal and sets the allergy value to true or false */
void check_allergy(char *question, int *pref_value) {
    printf("%s", question);   
    *pref_value = yes_no_prompt();
}

char fetch_tag(int i) {
    switch(i) {
        case 1: return GLUTEN_TAG;
        case 2: return LACTOSE_TAG;
        case 3: return NUT_TAG;
        case 4: return VEGAN_TAG;
        case 5: return QUICK_MEALS_TAG;
        case 6: return SLOW_MEALS_TAG;
        case 7: return HIGH_PROTEIN_TAG;
        case 8: return LOW_FAT_TAG;
    }
    return -1;
}

int get_user_prefs_value(int i) {
    switch(i) {
        case 0: return saved_user_prefs.gluten_allergy;
        case 1: return saved_user_prefs.lactose_intolerance;
        case 2: return saved_user_prefs.nut_allergy;
        case 3: return saved_user_prefs.vegan;
        case 4: return saved_user_prefs.quick_meals;
        case 5: return saved_user_prefs.slow_meals;
        case 6: return saved_user_prefs.high_protein;
        case 7: return saved_user_prefs.low_fat;
    }
    return -1;
}

void generate_user_tags(void) {
    /* Gå igennem alle user prefs og lav streng kat */
    int i;
    char *user_tags_buffer = calloc(sizeof(char), MAX_TAGS);

    for (i = 0; i < NUMBER_OF_USER_PREFS; i++) {
        if(get_user_prefs_value(i)) {
            int len = strlen(user_tags_buffer);
            user_tags_buffer[len] = fetch_tag(i + 1);
        }
    }

    user_prefs_tags = calloc(sizeof(char), strlen(user_tags_buffer));
    strcpy(user_prefs_tags, user_tags_buffer);    

    free(user_tags_buffer);
}

void meal_pref_status(int index, char *text, int pref_value) {
    int i;
    int text_length = strlen(text);
    int dot_length = SPACING - text_length;
    char *dots = calloc(sizeof(char), dot_length);

    for (i = 0; i < dot_length - 1; i++)
        dots[i] = '.';
        
    printf("%d. %s%s %s\n", index, text, dots, (pref_value == 1) ? "Ja" : "Nej");

    free(dots);
}

void print_current_user_prefs(void) {
    meal_pref_status(1, "Jeg har gluten allergi", saved_user_prefs.gluten_allergy);
    meal_pref_status(2, "Jeg har laktose intolerans", saved_user_prefs.lactose_intolerance);
    meal_pref_status(3, "Jeg har noedeallergi", saved_user_prefs.nut_allergy);
    meal_pref_status(4, "Jeg foretraekker kun veganske maaltider", saved_user_prefs.vegan);
    meal_pref_status(5, "Jeg foretraekker kun maaltider med under 20 minutters tilberedelsestid", saved_user_prefs.quick_meals);
    meal_pref_status(6, "Jeg foretraekker kun maaltider med over 20 minutters tilberedelsestid", saved_user_prefs.slow_meals);
    meal_pref_status(7, "Jeg foretraekker kun maaltider med hoejt protein indhold", saved_user_prefs.high_protein);
    meal_pref_status(8, "Jeg foretraekker kun maaltider med lavt fedt inhold", saved_user_prefs.low_fat);
    printf("\n");
}

void change_preference_value(int choice) {
    int quick_meals_before = saved_user_prefs.quick_meals;
    int slow_meals_before = saved_user_prefs.slow_meals;

    switch (choice) {
        case 1: saved_user_prefs.gluten_allergy = !saved_user_prefs.gluten_allergy; break;
        case 2: saved_user_prefs.lactose_intolerance = !saved_user_prefs.lactose_intolerance; break;
        case 3: saved_user_prefs.nut_allergy = !saved_user_prefs.nut_allergy; break;
        case 4: saved_user_prefs.vegan = !saved_user_prefs.vegan; break;
        case 5: saved_user_prefs.quick_meals = !saved_user_prefs.quick_meals; break;
        case 6: saved_user_prefs.slow_meals = !saved_user_prefs.slow_meals; break;
        case 7: saved_user_prefs.high_protein = !saved_user_prefs.high_protein; break;
        case 8: saved_user_prefs.low_fat = !saved_user_prefs.low_fat; break;
    }
    
    /* Make sure the user doesn't have both quick and slow meals enabled */
    if (saved_user_prefs.slow_meals && saved_user_prefs.quick_meals) {
        saved_user_prefs.quick_meals = !quick_meals_before;
        saved_user_prefs.slow_meals = !slow_meals_before;
    }
}

void prompt_for_preference_to_change(int *choice) {
    int max_index = 8;
    printf("Hvilken praeference vil du aendre? Tast '0' for at gaa tilbage.\n");
    
    *choice = prompt_for_index_to_change(max_index);
}

void change_user_preferences(void) {
    int choice = -1;

    do {    
        system("cls");

        print_current_user_prefs();
        prompt_for_preference_to_change(&choice);
        change_preference_value(choice);
    } while (choice != 0);

    save_user_prefs();
    generate_user_tags();
    system("cls");
}