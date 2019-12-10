#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../utility/utility.h"
#include "database.h"

const char *dir_name = "./recipes/";

int count_recipe_files(void) {
    int file_count = 0;
    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(dir_name); /* There should be error handling after this */

    while ((entry = readdir(dirp)) != NULL)
        if (entry->d_type == DT_REG) /* If the entry is a regular file */
            file_count++;

    closedir(dirp);
    return file_count;
}

void get_file_names(char **file_names) {
    /* https://www.daniweb.com/programming/software-development/threads/99538/saving-file-names-from-directory-to-array */
    int i = 0;
    struct dirent *dirent;
    DIR *dir = opendir(dir_name);    

    if(dir) {
        while ((dirent = readdir(dir)) != NULL) {
            if(strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
                file_names[i] = malloc(strlen(dirent->d_name) + 1);
                strcpy(file_names[i], dirent->d_name);
                i++;
            }
        }
        closedir(dir);
    }
}

int count_ingredients_from_file(FILE *fp) {
    int ch, number_of_lines = 0;
    do {
        ch = fgetc(fp);
        if (ch == '\n' || ch == ';')
            number_of_lines++;
    } while (ch != ';');

    return number_of_lines;
}

ingredient *get_ingredients(FILE *fp) {
    char line[MAX_INGREDIENTS_CHARS];
    int sentinel = 0, i = 0,
        ingredient_start_position = ftell(fp),
        ingredients_in_file = count_ingredients_from_file(fp);

    ingredient *ingredients = calloc(sizeof(ingredient), ingredients_in_file);

    /* Reset cursor back to the beginning of the ingredient list */
    fseek(fp, ingredient_start_position, SEEK_SET);

    while (fgets(line, sizeof(line), fp) && !sentinel) {
        ingredient ingredient;
        char name_buffer[MAX_LINE_LENGTH];
        char unit_buffer[MAX_LINE_LENGTH];
        int end_of_string;

        sscanf(line, "%*c %f %s %[^\n]s", &ingredient.amount, unit_buffer, name_buffer);

        end_of_string = strlen(name_buffer) - 1;

        if (name_buffer[end_of_string] == ';') {
            name_buffer[end_of_string] = '\0';
            sentinel = 1;
        }

        ingredient.name = calloc(sizeof(char), strlen(name_buffer));
        ingredient.unit = calloc(sizeof(char), strlen(unit_buffer));

        strcpy(ingredient.name, name_buffer);
        strcpy(ingredient.unit, unit_buffer);

        /*printf("%.2f %s %s\n", ingredient.amount, ingredient.unit, ingredient.name);*/

        ingredients[i] = ingredient;
        i++;
    }
    return ingredients;
}

recipe get_recipe_data(FILE *fp, const char *file_name) {
    /* Temporary static array declarations */
    char name_buffer[MAX_NAME_LENGTH];
    char procedure_buffer[MAX_PROCEDURE_CHARS];
    char tags_buffer[MAX_TAGS];

    recipe recipe;

    fscanf(fp, "%[^;];\n", name_buffer);

    recipe.ingredients = get_ingredients(fp);
    /*printf("------------------------------\n"); */

    /* Scan through the file until the symbol ; is encountered for each of the four strings */
    /* Also note how recipe.time is written to directly, because it is easy to assign to */

    fscanf(fp, "%[^;]; %[^;]; %d", procedure_buffer, tags_buffer, &recipe.time);

    /* Allocate memory for the recipe struct */
    recipe.name =      calloc(strlen(name_buffer) + 1,      sizeof(char));
    recipe.procedure = calloc(strlen(procedure_buffer) + 1, sizeof(char));
    recipe.tags =      calloc(strlen(tags_buffer) + 1,      sizeof(char));
    recipe.file_name = calloc(strlen(file_name) + 1,        sizeof(char));
    
    /* Copy the items into the struct */
    strcpy(recipe.name, name_buffer);
    strcpy(recipe.procedure, procedure_buffer);
    strcpy(recipe.tags, tags_buffer);
    strcpy(recipe.file_name, file_name);

    /* Print the result */
    /*printf("Title:\n- %s\n\nProcedure:\n%s\n\nTags: %s\n\nTime: %d minutes.\n",
           recipe.name, recipe.procedure, recipe.tags, recipe.time);*/

    return recipe;
}

char *get_file_directory(const char *file_name) {
    /* Allocate memory for the directory name */
    char *directory_name = calloc(strlen(dir_name) + strlen(file_name), sizeof(char));

    /* Concatenate the recipe directory with the name of the file and the .txt extension */
    /* Format: ./recipes/fileName.txt where the dot represents the current directory */
    strcpy(directory_name, dir_name);
    strcat(directory_name, file_name);

    return directory_name;
}

recipe *get_database(char **file_names, int file_count) {
    /* Get the amount of files to read based on the files array defined at the top */
    int i;
    
    /* Initialize the array of recipe structs (the database) */
    recipe *recipe_database = malloc(sizeof(recipe) * file_count);

    for (i = 0; i < file_count; i++) {
        /* Initialize file variable and get the file directory as a string */
        FILE *fp;
        char *directory_name = get_file_directory(file_names[i]);

        /* Open the file using the directory name */
        fp = fopen(directory_name, "r");

        /* Print the data from the file */
        recipe_database[i] = get_recipe_data(fp, file_names[i]);

        /* Close the file stream after reading */
        fclose(fp);
    }
    return recipe_database;
}