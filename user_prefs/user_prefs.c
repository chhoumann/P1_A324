#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "user_prefs.h"

#define PRINT_SPACING 75

/* Prototype methods */
void initial_setup(void);
void read_user_prefs_from_file(FILE *file);
void save_user_prefs(void);
void check_allergy(char *question, int *pref_value);
void print_user_pref_value(int index, char *text, int pref_value);
void print_all_user_prefs(void);
void prompt_for_preference_to_change(int *choice);
void generate_user_tags(void);
void change_preference_value(int choice);
char fetch_tag(int i);
int get_user_prefs_value(int i);

/* The path of the user preferences .txt file */
const char *user_prefs_file_name = "./user_prefs.txt";

/* Main function that keeps prompting for user preferences changes until the user inputs 0 (sentinel) */
void change_user_preferences(void) {
    int choice = -1;

    do {    
        clear_screen();

        print_all_user_prefs();
        prompt_for_preference_to_change(&choice);
        change_preference_value(choice);
    } while (choice != 0);

    /* When the user chooses to exit, we save the current settings and generate a tag string */
    save_user_prefs();
    generate_user_tags();
    clear_screen();
}

/* Runs the initial setup if the setup file is empty or does not exist */
void check_setup(void) {
    FILE *user_prefs_file = fopen(user_prefs_file_name, "r");

    if (user_prefs_file == NULL || user_prefs_file != NULL && is_file_empty(user_prefs_file))
        initial_setup();
    else
        read_user_prefs_from_file(user_prefs_file);
    
    generate_user_tags();
}

/* Prompts the user for allergies and calls the save user preferences method */
void initial_setup(void) {
    printf("Velkommen! Foer vi gaar i gang, skal vi lige vide, om du har nogle allergier.\n");
    check_allergy("Har du glutenallergi? (y/n) ", &saved_user_prefs.gluten_allergy);
    check_allergy("Er du laktoseintolerant? (y/n) ", &saved_user_prefs.lactose_intolerance);
    check_allergy("Har du noeddeallergi? (y/n) ", &saved_user_prefs.nut_allergy);
    
    /* Set default values on initial run */
    saved_user_prefs.vegan = 0; saved_user_prefs.quick_meals = 0; saved_user_prefs.slow_meals = 0; saved_user_prefs.low_fat = 0;
    
    /* Set high protein to true by default because high protein meals are better when focusing on health (see report) */
    saved_user_prefs.high_protein = 1; 
    
    save_user_prefs();
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

/* Prints the current values of the user preferences spaced with dots */
void print_user_pref_value(int index, char *text, int pref_value) {
    int i;
    int text_length = strlen(text);
    int dot_length = PRINT_SPACING - text_length;
    char *dots = calloc(sizeof(char), dot_length);

    for (i = 0; i < dot_length - 1; i++)
        dots[i] = '.';
        
    printf("%d. %s%s %s\n", index, text, dots, (pref_value == 1) ? "Ja" : "Nej");

    free(dots);
}

/* Calls a print for every user preferences  */
void print_all_user_prefs(void) {
    print_user_pref_value(1, "Jeg har glutenallergi", saved_user_prefs.gluten_allergy);
    print_user_pref_value(2, "Jeg har laktoseintolerans", saved_user_prefs.lactose_intolerance);
    print_user_pref_value(3, "Jeg har noeddeallergi", saved_user_prefs.nut_allergy);
    print_user_pref_value(4, "Jeg foretraekker kun veganske maaltider", saved_user_prefs.vegan);
    print_user_pref_value(5, "Jeg foretraekker kun maaltider med under 20 minutters tilberedningstid", saved_user_prefs.quick_meals);
    print_user_pref_value(6, "Jeg foretraekker kun maaltider med over 20 minutters tilberedningstid", saved_user_prefs.slow_meals);
    print_user_pref_value(7, "Jeg foretraekker kun maaltider med hoejt proteinindhold", saved_user_prefs.high_protein);
    print_user_pref_value(8, "Jeg foretraekker kun maaltider med lavt fedtindhold", saved_user_prefs.low_fat);
    printf("\n");
}

/* Ask the user which user they would like to change and sets the value of choice to the user's input (1-8) */
void prompt_for_preference_to_change(int *choice) {
    int max_index = 8;
    printf("Hvilken praeference vil du aendre? Tast '0' for at gaa tilbage.\n");
    
    *choice = prompt_for_index_to_change(max_index);
}

/* Goes through every tag and generates a string containing the tags corresponding to the set user preferences
   For example, one might have "!<" corresponding to a preference of high protein and quick meals */
void generate_user_tags(void) {
    int i = 0;
    char tag;
    char *user_tags_buffer = calloc(sizeof(char), MAX_TAG_BUFFER_LENGTH);

    /* Places the ith tag at the end of the buffer while tag is greater than 0 */
    do {
        int pref_value = get_user_prefs_value(i);        
        tag = fetch_tag(i);
        
        if (pref_value && tag > 0)
            user_tags_buffer[strlen(user_tags_buffer)] = tag;
        
        i++;
    } while(tag > 0);

    user_prefs_tags = calloc(sizeof(char), strlen(user_tags_buffer));
    strcpy(user_prefs_tags, user_tags_buffer);    

    free(user_tags_buffer);
}

/* Swaps the value of the given user preference (choice) to the opposite value */
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

/* Simple switch case returning the given tag based on input parameter */
char fetch_tag(int i) {
    switch(i) {
        case 0: return GLUTEN_TAG;
        case 1: return LACTOSE_TAG;
        case 2: return NUT_TAG;
        case 3: return VEGAN_TAG;
        case 4: return QUICK_MEALS_TAG;
        case 5: return SLOW_MEALS_TAG;
        case 6: return HIGH_PROTEIN_TAG;
        case 7: return LOW_FAT_TAG;
    }
    return -1;
}

/* Returns the value (true/false) of setting based on input parameter */
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