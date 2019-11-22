#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_INGREDIENTS_CHARS 1024
#define MAX_PROCEDURE_CHARS 1024
#define MAX_TAGS 10

typedef enum { g, kg } unit;
const char *files[] = { "test", "test2" };
const char *dir = "./recipes/";

/* Recipe struct which contains all the info */
typedef struct {
    char *name;
    char *procedure;
    char *ingredients;
    char *tags;
    int time;
} recipe;

recipe *get_database();
char *get_file_directory(const char *file_name, const char *file_extension);

int main(void) {    
    /* Initialize the database */
    recipe *recipe_database = get_database();

    /* Example. Prints the name of the first recipe in the databaes. */
    printf("Name of the first recipe in the database is: %s.\n", recipe_database[0].name);

    /* When we're done with the database, we need to free the memory */
    free(recipe_database);
    return EXIT_SUCCESS;
}

recipe get_recipe_data(FILE *fp) {
    recipe recipe;
    /*printf("------------------------------\n"); */

    /* Temporary static array declarations */
    char name_buffer[MAX_NAME_LENGTH];
    char ingredients_buffer[MAX_INGREDIENTS_CHARS];
    char procedure_buffer[MAX_PROCEDURE_CHARS];
    char tags_buffer[MAX_TAGS];

    /* Scan through the file until the symbol ; is encountered for each of the four strings */ 
    /* Also note how recipe.time is written to directly, because it is easy to assign to */
    fscanf(fp, "%[^;]; %[^;]; %[^;]; %[^;]; %d", name_buffer, ingredients_buffer, procedure_buffer, tags_buffer, &recipe.time);

    /* Allocate memory for the recipe struct */
    recipe.name        = calloc(strlen(name_buffer),        sizeof(char));
    recipe.ingredients = calloc(strlen(ingredients_buffer), sizeof(char));
    recipe.procedure   = calloc(strlen(procedure_buffer),   sizeof(char));
    recipe.tags        = calloc(strlen(tags_buffer),        sizeof(char));

    /* Copy the items into the struct */
    strcpy(recipe.name,        name_buffer);
    strcpy(recipe.ingredients, ingredients_buffer);
    strcpy(recipe.procedure,   procedure_buffer);
    strcpy(recipe.tags,        tags_buffer);

    /* Print the result
    printf("Title:\n- %s\n\nIngredients:\n%s\n\nProcedure:\n%s\n\nTags: %s\n\nTime: %d minutes.\n", 
    recipe.name, recipe.ingredients, recipe.procedure, recipe.tags, recipe.time); */
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