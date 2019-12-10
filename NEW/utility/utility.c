#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "utility.h"

#define TXT_EXTENSION_LENGTH 4

/* Checks whether the setup file exists or not (and if it exists, checks if it's empty) */
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

/* Clear input buffer */
void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* Get a recipe by file name (does not include file extension) */
int get_recipe_by_name(char *name, recipe *recipe_database, int number_of_recipes) {
    int i;

    for (i = 0; i < number_of_recipes; i++) {
        /* Get file name length and create new string to contain file name without .txt extension */
        int name_length = strlen(recipe_database[i].file_name);
        char *file_name_without_extension = calloc(name_length, sizeof(char));

        /* Make a new string without the .txt extension (i.e Wok.txt becomes Wok) */
        strncpy(file_name_without_extension, recipe_database[i].file_name, name_length - TXT_EXTENSION_LENGTH);

        /* If recipe was found, return index */
        if (strcmp(file_name_without_extension, name) == 0)
            return i;

        /* Free memory allocation */
        free(file_name_without_extension);
    }

    return -1;
}