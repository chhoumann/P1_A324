#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "weekplan.h"

/* Global variable for weekplan directory name for ease of access */
const char *weekplan_directory = "./weekplan/saved_weekplan.txt";

void prompt_for_weekplan_change(int *day);
void prompt_for_meal_to_change(int *selected_meal);

/* Returns true if the weekplan exists */
int weekplan_exists(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "r");

    if (weekplan_file != NULL && !is_file_empty(weekplan_file)) {
        fclose(weekplan_file);
        return 1;
    }
    
    return 0;
}

/* Loads a weekplan from the saved_weekplan.txt file using get_recipe_by_file_name
   (returns the index of the recipe in the database) */
int *load_weekplan(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "r");
    char recipe_name[MAX_RECIPE_NAME_LENGTH];
    int i;

    weekplan = calloc(sizeof(int), DAYS_IN_WEEK);

    /* Scans the weekplan file and finds the index in the database corresponding to every recipe name in the file */
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        fscanf(weekplan_file, "%[^\n]\n", recipe_name);
        weekplan[i] = get_recipe_by_file_name(recipe_name);
    }

    fclose(weekplan_file);

    return weekplan;
}

/* Saves a weekplan to the .txt file using integers and accesing the database 
   (format is simply recipe_name1\nrecipe_name2 etc.) */
void save_weekplan(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "w");
    int i;
    
    for (i = 0; i < DAYS_IN_WEEK; i++)
        fprintf(weekplan_file, "%s\n", recipe_database[weekplan[i]].file_name);

    fclose(weekplan_file);
}

/* Prints the currently saved weekplan - fetches recipe name from database using the indexes of the weekplan array */
void print_current_weekplan(void) {
    int i;

    const char *weekday_names[DAYS_IN_WEEK] = { "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Loerdag", "Soendag" };

    for (i = 0; i < DAYS_IN_WEEK; i++)
        printf("%d. %7s: %s.\n", i + 1, weekday_names[i], recipe_database[weekplan[i]].name); 
}

/* Prints all recipes inside the weekplan and prompts the user to select one to change it */
void print_weekplan_recipe(void) {
    int choice = -1;
    float serving_size;

    system("cls");
    
    print_current_weekplan();
    printf("Vaelg en opskrift (1-7) for at vise detaljer om den. Skriv '0' for at gaa tilbage.\n");

    choice = prompt_for_index_to_change(DAYS_IN_WEEK);
    system("cls");

    serving_size = get_serving_size();

    if (choice == 0)
        return;
    else if (choice != -1)
        print_recipe(recipe_database[weekplan[choice - 1]], serving_size);

    press_any_key_to_continue(); 
    system("cls");
}

/* Change the currently saved weekplan */
void change_weekplan(void) {
    int selected_meal = 0, more_changes = 0;
    
    do {
        /* Declare day variable and reset more_changes (loop exit condition) */
        int day = 0;
        more_changes = 0;

        system("cls");
        prompt_for_weekplan_change(&day);

        if (day != 0) {
            /* User selected a day, ask them what meal to change */
            prompt_for_meal_to_change(&selected_meal);

            /* Subtracting one from the day and selected_meal integers because it has to equal the array-indices from 0 to 6 */
            day--; selected_meal--;
            weekplan[day] = selected_meal;

            printf("\n\"%s\" er blevet erstattet med \"%s\".\n", recipe_database[weekplan[day]].name, recipe_database[selected_meal].name);        
            printf("\nOensker du foretage flere aendringer? (y/n)\n");

            more_changes = yes_no_prompt();
        }
    } while (more_changes == 1);

    /* Save changes */
    save_weekplan();
    system("cls");
}

/* Display the current weekplan and ask the user which recipe to replace */
void prompt_for_weekplan_change(int *day) {
    printf("Din nuvaerende ugeplan er:\n");
    print_current_weekplan();

    printf("\nHvilken dag vil du gerne aendre? Tryk '0' for at ikke aendre noget.\n");
    *day = prompt_for_index_to_change(DAYS_IN_WEEK);
}

/* Prints every recipe in the database and prompts to user to pick one */
void prompt_for_meal_to_change(int *selected_meal) {
    int i;

    system("cls");
    printf("Hvilken af disse maaltider vil du erstatte?\n");
    for (i = 0; i < number_of_recipes; i++)
        printf("%2d. %s\n", i + 1, recipe_database[i].name);

    *selected_meal = prompt_for_index_to_change(DAYS_IN_WEEK);
}

/* In case the weekplan already exists, we ask the user if they want to replace the current one with a new one */
void new_weekplan_prompt(void) {
    if (weekplan_exists()) {
        printf("Er du sikker paa, at du vil generer en ny madplan og overskrive den nuvaerende? (y/n)\n");
        
        if (yes_no_prompt())
            make_random_weekplan();
    } 
    else {
        make_random_weekplan();
        printf("Made new weekplan successfully\n");
    }

    press_any_key_to_continue();
    system("cls");
}