#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utility/utility.h"
#include "setup.h"

#define MAX_NAME_LENGTH 50
#define MAX_INGREDIENTS_CHARS 256
#define MAX_PROCEDURE_CHARS 1024
#define MAX_TAGS 10

const char *files[] = { "test" };
const char *dir = "./recipes/";

recipe *get_database();
char *get_file_directory(const char *file_name, const char *file_extension);

int main(void) {
    /* Initialize the database */
    recipe *recipe_database = get_database();

    /* Example. Prints the name of the first recipe in the databaes. */
    //printf("Name of the first recipe in the database is: %s.\n", recipe_database[0].name);

    /* When we're done with the database, we need to free the memory */
    printf("Making random weekplan\n");
    make_random_weekplan(recipe_database, "!", 20);
    printf("Done\n");

    free(recipe_database);
    return EXIT_SUCCESS;
}

int count_ingrdients_from_file(FILE *fp) {
    int ch, number_of_lines = 0;
    do {
        ch = fgetc(fp);
        if (ch == '\n'|| ch == ';')
            number_of_lines++;
    } while (ch != ';');

    return number_of_lines;
}

ingredient *get_ingredients(FILE *fp, recipe recipe) {
    char line[MAX_INGREDIENTS_CHARS];
    int sentinel = 0,
        i = 0, 
        ingredient_start_position = ftell(fp),
        ingredients_in_file = count_ingrdients_from_file(fp);
    
    ingredient *ingredients = calloc(sizeof(ingredient), ingredients_in_file);

    /* Reset cursor back to the beginning of the ingredient list */
    fseek(fp, ingredient_start_position, 0);

    while(fgets(line, sizeof(line), fp) && !sentinel) {
        ingredient ingredient;
        char name_buffer[MAX_NAME_LENGTH];
        char unit_buffer[MAX_NAME_LENGTH];
        int end_of_string;

        sscanf(line, "%*c %f %s %[^\n]s", &ingredient.amount, unit_buffer, name_buffer);

        end_of_string = strlen(name_buffer) - 1;

        if(name_buffer[end_of_string] == ';') {
            name_buffer[end_of_string] = '\0';
            sentinel = 1;
        }

        ingredient.name = calloc(sizeof(char), strlen(name_buffer));
        ingredient.unit = calloc(sizeof(char), strlen(unit_buffer));

        strcpy(ingredient.name, name_buffer);
        strcpy(ingredient.unit, unit_buffer);

        printf("%f %s %s\n", ingredient.amount, ingredient.unit, ingredient.name);

        ingredients[i] = ingredient;
        i++;
    }
    return ingredients;
}

recipe get_recipe_data(FILE *fp) {
    /* Temporary static array declarations */
    char name_buffer[MAX_NAME_LENGTH];
    char procedure_buffer[MAX_PROCEDURE_CHARS];
    char tags_buffer[MAX_TAGS];

    recipe recipe;

    fscanf(fp, "%[^;];\n", name_buffer);
    printf("Recipe name: %s\n", name_buffer);

    recipe.ingredients = get_ingredients(fp, recipe);
    /*printf("------------------------------\n"); */

    /* Scan through the file until the symbol ; is encountered for each of the four strings */ 
    /* Also note how recipe.time is written to directly, because it is easy to assign to */
    fscanf(fp, "%[^;]; %[^;]; %d", procedure_buffer, tags_buffer, &recipe.time);

    /* Allocate memory for the recipe struct */
    recipe.name             = calloc(strlen(name_buffer),        sizeof(char));
    recipe.procedure        = calloc(strlen(procedure_buffer),   sizeof(char));
    recipe.tags             = calloc(strlen(tags_buffer),        sizeof(char));

    /* Copy the items into the struct */
    strcpy(recipe.name,      name_buffer);
    strcpy(recipe.procedure, procedure_buffer);
    strcpy(recipe.tags,      tags_buffer);

    /* Print the result */
    printf("Title:\n- %s\n\nProcedure:\n%s\n\nTags: %s\n\nTime: %d minutes.\n", 
    recipe.name, recipe.procedure, recipe.tags, recipe.time); 
    return recipe;
}

char *get_file_directory(const char *file_name, const char *file_extension) {
    /* Allocate memory for the directory name */
    char *directory_name = calloc(strlen(dir) + strlen(file_name) + strlen(file_extension), sizeof(char));
    
    /* Concatenate the recipe directory with the name of the file and the .txt extension */ 
    /* Format: ./recipes/fileName.txt where the dot represents the current directory */
    strcpy(directory_name, dir);
    strcat(directory_name, file_name);
    strcat(directory_name, file_extension);
    
    return directory_name;
}

recipe *get_database() {
    /* Get the amount of files to read based on the files array defined at the top */
    int i, n = sizeof(files) / sizeof(files[0]);

    /* Initialize the array of recipe structs (the database) */
    recipe *recipe_database = malloc(sizeof(recipe) * n);

    for (i = 0; i < n; i++) {
        /* Initialize file variable and get the file directory as a string */
        FILE *fp;
        char *fileName = get_file_directory(files[i], ".txt");

        /* Open the file using the directory name */
        fp = fopen(fileName, "r");

        /* Print the data from the file */
        recipe_database[i] = get_recipe_data(fp);
        
        /* Close the file stream after reading */
        fclose(fp);
    }
    return recipe_database;
}