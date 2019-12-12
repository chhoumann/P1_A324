#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "weekplan.h"

const char *weekplan_directory = "./weekplan/saved_weekplan.txt";

void prompt_for_weekplan_change(int *day);
void prompt_for_new_recipe(int *day, int *selected_meal);
void prompt_for_more_changes(int *more_changes);

/* Returns true if the weekplan exists */
int weekplan_exists(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "r");

    if (weekplan_file != NULL && !is_file_empty(weekplan_file)) {
        fclose(weekplan_file);
        return 1;
    }
    
    return 0;
}

/* Loads a weekplan from the saved_weekplan.txt file using get_recipe_by_file_name (returns the index of the recipe in the database) */
int *load_weekplan(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "r");
    char recipe_name[MAX_RECIPE_NAME_LENGTH];
    int i;

    weekplan = calloc(sizeof(int), DAYS_IN_WEEK);

    for (i = 0; i < DAYS_IN_WEEK; i++) {
        fscanf(weekplan_file, "%[^\n]\n", recipe_name);
        weekplan[i] = get_recipe_by_file_name(recipe_name);
    }

    fclose(weekplan_file);

    return weekplan;
}

/* Saves a weekplan to the .txt file using integers and accesing the database (format is simply recipe_name1\nrecipe_name2 etc.) */
void save_weekplan(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "w+");
    int i;
    
    for (i = 0; i < DAYS_IN_WEEK; i++)
        fprintf(weekplan_file, "%s\n", recipe_database[weekplan[i]].file_name);

    fclose(weekplan_file);
}

/* Prints the currently saved weekplan */
void print_current_weekplan(void) {
    int i;
    
    for (i = 0; i < DAYS_IN_WEEK; i++)
        printf("%d. %s.\n", i + 1, recipe_database[weekplan[i]].name); 
}

/* Prints all recipes inside the weekplan and prompts the user to select one to change it */
void print_weekplan_recipe(void) {
    int i, choice = -1;
    system("cls");
    
    print_current_weekplan();

    printf("Vaelg en opskrift (1-7) for at vise detaljer om den. Skriv '0' for at gaa tilbage.\n");

    do {
        scanf(" %d", &choice);

        if (choice >= 1 && choice <= 7)
            print_recipe(recipe_database[weekplan[choice - 1]]);
        else if (choice == 0)
            return;
        else
            on_invalid_input();

        clear_input_buffer();
    } while (choice == -1);

    press_any_key_to_continue();
}

/* Change the currently saved weekplan */
void change_weekplan(void) {
    int i, day, selected_meal = 0, more_changes = 1;
    
    while (more_changes == 1) {
        system("cls");

        prompt_for_weekplan_change(&day);
        prompt_for_new_recipe(&day, &selected_meal);

        /* Subtracting one from the day and selected_meal integers because it has to equal the array-indices from 0 to 6 */
        day--; selected_meal--;

        printf("%s er blevet erstattet med %s.\n", recipe_database[weekplan[day]].name, recipe_database[selected_meal]);        
        printf("\nOensker du foretage flere aendringer? (y/n)\n");
        
        weekplan[day] = selected_meal;

        prompt_for_more_changes(&more_changes);
    }

    /* Save changes */
    save_weekplan();
}

/* Display the current weekplan and ask the user which recipe to replace */
void prompt_for_weekplan_change(int *day) {
    int i;
    printf("Din nuvaerende ugeplan er:\n");
    print_current_weekplan();

    printf("\nHvilken dag vil du gerne aendre?\n");
    scanf(" %d", day);

    printf("Hvilken af disse maaltider vil du erstatte?\n");
    for (i = 0; i < number_of_recipes; i++)
        printf("%2d. %s\n", i + 1, recipe_database[i].name);
}

/* Display all recipes available and ask which one the weekplan recipe should be replaced with */
void prompt_for_new_recipe(int *day, int *selected_meal) {
    do {
        printf("Vaelg en ret: ");
        scanf(" %d", selected_meal);

        if (*selected_meal <= 0 || *selected_meal > number_of_recipes) {
            *selected_meal = 0;
            on_invalid_input();
            clear_input_buffer();
        }
    
    } while (selected_meal == 0);
}

/* Ask the user if they want to change any other recipes in the weekplan */
void prompt_for_more_changes(int *more_changes) {
    char choice;

    do {
        scanf(" %c", &choice);
        if (choice == 'n')
            *more_changes = 0;
        else if (choice == 'y')
            *more_changes = 1;
        else
            on_invalid_input();

        clear_input_buffer();

    } while (choice != 'n' && choice != 'y');
}