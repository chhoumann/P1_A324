#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "weekplan.h"

#define MAX_NAME_LENGTH 50

const char *weekplan_directory = "./saved_weekplan.txt";

/* Returns true if the weekplan exists */
int weekplan_exists(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "r");

    if (weekplan_file != NULL && !is_file_empty(weekplan_file)) {
        fclose(weekplan_file);
        return 1;

    }
    
    return 0;
}

/* Loads a weekplan from the .txt file and returns a struct with all the names filled out */
recipe *load_weekplan(void) {
    FILE *weekplan_file = fopen(weekplan_directory, "r");
    recipe *weekplan = calloc(sizeof(recipe), DAYS_IN_WEEK);
    int i;
    char name_buffer[MAX_NAME_LENGTH];

    for (i = 0; i < DAYS_IN_WEEK; i++) {
        fscanf(weekplan_file, "%s ", name_buffer);

        weekplan[i].file_name = calloc(sizeof(char), strlen(name_buffer));
        strcpy(weekplan[i].file_name, name_buffer);
    }

    fclose(weekplan_file);

    return weekplan;
}

/* Saves a weekplan to the .txt file (format is simply recipe_name1 recipe_name2 etc.) */
void save_weekplan(recipe *weekplan) {
    FILE *weekplan_file = fopen(weekplan_directory, "w+");
    int i;
    
    for (i = 0; i < DAYS_IN_WEEK; i++)
        fprintf(weekplan_file, "%s ", weekplan[i].file_name);
}