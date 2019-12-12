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
void clear_input_buffer(void) {
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

int yes_no_prompt(void) {
    char choice;
    int more_changes;

    do {
        scanf(" %c", &choice);
        if (choice == 'n')
            more_changes = 0;
        else if (choice == 'y')
            more_changes = 1;
        else
            on_invalid_input();

        clear_input_buffer();

    } while (choice != 'n' && choice != 'y');

    return more_changes;
}

void on_invalid_input(void) {
    printf("Ugyldigt input, proev igen.\n");
}

void print_recipe(recipe recipe) {
    int i;

    system("cls");
    printf("\n%s\nTid: %d\n", recipe.name, recipe.time);

    /* Print ingredients */
    printf("Ingredienser:\n");
    for (i = 0; i < recipe.number_of_ingredients; i++)
        printf("- %.2f %s %s\n", recipe.ingredients[i].amount, recipe.ingredients[i].unit, recipe.ingredients[i].name);

    printf("\nFremangsmaade:\n%s\n\n", recipe.procedure);
}

void press_any_key_to_continue(void) {
    printf("Tryk paa en vilkaarlig tast paa tastaturet for at returnere til menuen.\n");
    getchar();
}