#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WEEK_LENGTH 7
#define MAX_NAME_LENGTH 50

/* Recipe struct which contains all the info */
typedef struct {
    char *name;
    char *procedure;
    char *ingredients;
    char *tags;
    int time;
} recipe;

recipe *load_weekplan(FILE *weekplan_file);
void save_weekplan(recipe *weekplan);
int is_file_empty(FILE *file);
recipe make_recipe(char *name);

const char *dir = "./saved_weekplan.txt";

int main(void) {
    FILE *weekplan_file = fopen(dir, "r");

    if(weekplan_file != NULL && !is_file_empty(weekplan_file))
        /* Load the week plan if it's not empty */
        load_weekplan(weekplan_file);
    else {
        /* Makes a weekplan and saves its names - in the future, we call it after a randomization or edit has been made */
        recipe *weekplan = calloc(sizeof(recipe), WEEK_LENGTH);

        weekplan[0] = make_recipe("kebabrulle");
        weekplan[1] = make_recipe("kebabsovs");
        weekplan[2] = make_recipe("koedsovs");
        weekplan[3] = make_recipe("ulle");
        weekplan[4] = make_recipe("Boller");
        weekplan[5] = make_recipe("Pizza");
        weekplan[6] = make_recipe("bagekartofler");
        save_weekplan(weekplan);
    }
}

/* Loads a weekplan from the .txt file and returns a struct with all the names filled out */
recipe *load_weekplan(FILE *weekplan_file) {
    int i;
    char name_buffer[MAX_NAME_LENGTH];
    recipe *weekplan = calloc(sizeof(recipe), WEEK_LENGTH);

    for (i = 0; i < WEEK_LENGTH; i++) {
        fscanf(weekplan_file, "%s ", name_buffer);

        weekplan[i].name = calloc(sizeof(char), strlen(name_buffer));
        strcpy(weekplan[i].name, name_buffer);
    }

    printf("%s", weekplan[0].name);

    return weekplan;
}

/* Saves a weekplan to the .txt file (format is simply recipe_name1 recipe_name2 etc.) */
void save_weekplan(recipe *weekplan) {
    FILE *weekplan_file = fopen(dir, "w+");
    int i;
    
    for (i = 0; i < WEEK_LENGTH; i++)
        fprintf(weekplan_file, "%s ", weekplan[i].name);
}

/* To make test data */
recipe make_recipe(char *name) {
    recipe result;
    
    result.name = calloc(sizeof(char), strlen(name));
    strcpy(result.name, name);
    
    return result;
}

/* Checks whether the setup file exists or not (and if it exists, checks if it's empty) - UTILITY METHOD */
int is_file_empty(FILE *file) {
    /* Check if file is empty */
    int file_size;
    
    /* Set the file cursor position to the end of the file and read file size in bytes */
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);

    if (file_size == 0) {
        /* File exists, but is empty - also reset the cursor to the start of the file */
        fseek(file, 0, SEEK_SET);
        return 1;
    }

    /* File is not empty - also reset the cursor to the start of the file */
    fseek(file, 0, SEEK_SET);
    return 0;
}