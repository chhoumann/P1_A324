#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../utility/utility.h"
#include "database.h"

/* The directory name is global to allow ease of access without parameter transfers across files */
const char *database_dir_name = "./recipes/";

/* Prototype methods */
recipe get_recipe_data(FILE *fp, const char *file_name);
int count_ingredients_from_file(FILE *fp);
char *get_file_directory(const char *file_name);
void get_ingredients(FILE *fp, recipe *recipe);

/* Main database getter. Sets the global variable - called in main.c */
void get_database(char **file_names) {
    int i;
    
    /* Initialize the array of recipe structs (the database) */
    recipe_database = calloc(sizeof(recipe), number_of_recipes);
    
    for (i = 0; i < number_of_recipes; i++) {
        /* Initialize file variable and get the file directory as a string */
        FILE *fp;
        char *directory_name = get_file_directory(file_names[i]);
        
        /* Open the file using the directory name */
        fp = fopen(directory_name, "r");
        
        /* Print the data from the file */
        recipe_database[i] = get_recipe_data(fp, file_names[i]);

        /* Close the file stream after reading */
        fclose(fp);
        free(directory_name);
    }
}

recipe get_recipe_data(FILE *fp, const char *file_name) {
    /* Temporary static array declarations */
    char name_buffer[MAX_RECIPE_NAME_LENGTH];
    char procedure_buffer[MAX_PROCEDURE_CHARS];
    char tags_buffer[MAX_TAG_BUFFER_LENGTH];

    recipe recipe;

    /* Scan the name into the name buffer and initialize the ingredients array in the recipe */
    fscanf(fp, "%[^;];\n", name_buffer);
    get_ingredients(fp, &recipe);

    /* Scan through the file until the symbol ; is encountered for each of the four strings */
    /* Also note how recipe.time is written to directly, because it is easy to assign to */
    fscanf(fp, "%[^;]; %[^;]; %d", procedure_buffer, tags_buffer, &recipe.time);

    /* Allocate memory for the recipe struct's strings */
    recipe.name      = calloc(strlen(name_buffer) + 1,      sizeof(char));
    recipe.procedure = calloc(strlen(procedure_buffer) + 1, sizeof(char));
    recipe.tags      = calloc(strlen(tags_buffer) + 1,      sizeof(char));

    /* Set file_name of the recipe struct without the .txt extension (so it's easier to look up) */
    recipe.file_name = calloc(strlen(file_name) + 1, sizeof(char));
    strncpy(recipe.file_name, file_name, strlen(file_name) - 4);

    /* Copy the items into the struct */
    strcpy(recipe.name, name_buffer);
    strcpy(recipe.procedure, procedure_buffer);
    strcpy(recipe.tags, tags_buffer);

    return recipe;
}

char *get_file_directory(const char *file_name) {
    /* Allocate memory for the directory name */
    char *directory_name = calloc(strlen(database_dir_name) + strlen(file_name), sizeof(char));

    /* Concatenate the recipe directory with the name of the file and the .txt extension */
    /* Format: ./recipes/fileName.txt where the dot represents the current directory */
    strcpy(directory_name, database_dir_name);
    strcat(directory_name, file_name);

    return directory_name;
}

/* Counts the number of recipe .txt files in the recipes folder */
char **get_and_count_recipe_files(void) {
    int i = 0;
    struct dirent *dir_entry;
    char **file_names_buffer = calloc(MAX_RECIPES, sizeof(char*));
    char **file_names;
    DIR *dir = opendir(database_dir_name);

    number_of_recipes = 0;
    
    /* Error handling in case the directory cannot be opened */
    if (dir == NULL) {
        printf("Fejl: Aabning af databasekatalog fejlede. Afslutter.\n");
        press_any_key_to_continue();
        exit(EXIT_FAILURE);
    }

    /* Increment file_count by 1 for every regular file (DT_REG) in the folder */
    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_type == DT_REG) { /* Check if it's a regular file, i.e. not a folder for example */
            char *entry_name = dir_entry->d_name;

            /* Ignore the directory entry for current directory '.' 
               and the directory on level up in the hierarchy '..'  */
            if (strcmp(entry_name, ".") != 0 && strcmp(entry_name, "..") != 0) {

                /* Allocate memory and copy the name of the entry into the file name buffer */
                file_names_buffer[number_of_recipes] = calloc(strlen(entry_name) + 1, sizeof(char));
                strcpy(file_names_buffer[number_of_recipes], entry_name);
                
                number_of_recipes++;
            }
        }
    }
    
    /* Allocate memory for the file name array now that we know how many files are inside the directory */
    file_names = calloc(number_of_recipes, sizeof(char*));

    /* Copy all files names from the buffer into the file name array */
    for (i = 0; i < number_of_recipes; i++) {
        file_names[i] = calloc(strlen(file_names_buffer[i]) + 1, sizeof(char));
        strcpy(file_names[i], file_names_buffer[i]);
    }
    
    /* Close directory after reading has finished and free memory from file name buffer */
    closedir(dir);
    free(file_names_buffer);

    return file_names;
}

/* Places all the ingredients from a .txt file into the recipe struct assuming we've already read the name */
void get_ingredients(FILE *fp, recipe *recipe) {
    char line[MAX_INGREDIENTS_CHARS];
    int sentinel = 0, i = 0,
        ingredient_start_position = ftell(fp), /* Assumes current cursor position is after the recipe name has been read */
        ingredients_in_file = count_ingredients_from_file(fp);

    /* Allocate memory for the recipe's ingredients array since we now know the amount of ingredients */
    recipe->ingredients = calloc(sizeof(ingredient), ingredients_in_file);

    /* Reset cursor back to the beginning of the ingredient list */
    fseek(fp, ingredient_start_position, SEEK_SET);

    /* Reads ingredient data from recipe file line by line and stores in ingredients array */
    while (fgets(line, sizeof(line), fp) && !sentinel) {
        char name_buffer[MAX_LINE_LENGTH];
        char unit_buffer[MAX_LINE_LENGTH];
        int end_of_string;

        /* Scan the line - format example: "- 10 g himalayan pink salt"  */
        sscanf(line, "%*c %f %s %[^\n]s", &recipe->ingredients[i].amount, unit_buffer, name_buffer);

        /* Get end of string to check if it's the final ingredient (meaning it ends with ';') */
        end_of_string = strlen(name_buffer) - 1;

        if (name_buffer[end_of_string] == ';') {
            /* Final ingredient on the list is reached - exit loop once this iteration has finished */
            name_buffer[end_of_string] = '\0';
            sentinel = 1;
        }

        /* Allocate memory for the name and unit and copy from the buffer */
        recipe->ingredients[i].name = calloc(sizeof(char), strlen(name_buffer));
        recipe->ingredients[i].unit = calloc(sizeof(char), strlen(unit_buffer));

        strcpy(recipe->ingredients[i].name, name_buffer);
        strcpy(recipe->ingredients[i].unit, unit_buffer);

        i++;
    }

    /* Set number of ingredients to the index we reached */
    recipe->number_of_ingredients = i;
}

/* Count the amount of ingredients in a recipe file assuming we've already read the name */
int count_ingredients_from_file(FILE *fp) {
    int ch, number_of_lines = 0;

    /* If the next character in the file is either a newline or a semicolon number of lines is counted up.
       We use a semicolon to separate 'sections' of the recipes, hence the semicolon. 
       This ensures that we get the amount of ingredients from the file by counting the amount of lines. */
    do {
        ch = fgetc(fp);
        if (ch == '\n' || ch == ';')
            number_of_lines++;
    } while (ch != ';');

    return number_of_lines;
}