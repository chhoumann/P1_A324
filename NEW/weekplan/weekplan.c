#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "weekplan.h"

const char *weekplan_directory = "./weekplan/saved_weekplan.txt";

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
    int i, *weekplan = calloc(sizeof(int), DAYS_IN_WEEK);

    for (i = 0; i < DAYS_IN_WEEK; i++) {
        fscanf(weekplan_file, "%[^\n]\n", recipe_name);
        weekplan[i] = get_recipe_by_file_name(recipe_name);
    }

    fclose(weekplan_file);

    return weekplan;
}

/* Saves a weekplan to the .txt file using integers and accesing the database (format is simply recipe_name1\nrecipe_name2 etc.) */
void save_weekplan(int *weekplan) {
    FILE *weekplan_file = fopen(weekplan_directory, "w+");
    int i;
    
    for (i = 0; i < DAYS_IN_WEEK; i++)
        fprintf(weekplan_file, "%s\n", recipe_database[weekplan[i]].file_name);

    fclose(weekplan_file);
}