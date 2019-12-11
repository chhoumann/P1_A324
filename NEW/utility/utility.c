#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "utility.h"

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
int get_recipe_by_file_name(char *name) {
    int i;

    for (i = 0; i < number_of_recipes; i++) {
        /* If recipe was found, return index */
        if (strcmp(recipe_database[i].file_name, name) == 0)
            return i;
    }

    return -1;
}