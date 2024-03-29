#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <math.h>
#include <conio.h>
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

/* Simple yes/no prompt that returns 1 if the user enters y and 0 if they enters n */
int yes_no_prompt(void) {
    char input;
    int choice;

    do {
        scanf(" %c", &input);
        if (input == 'n')
            choice = 0;
        else if (input == 'y')
            choice = 1;
        else {
            /* Input not recognized, so we tell the user */
            on_invalid_input();
            clear_input_buffer();
        }
    } while (input != 'n' && input != 'y');

    return choice;
}

/* Used for menu selection. Allows for selection between 0 and max index based on a parameter */
int prompt_for_index_to_change(int max_index) {
    int choice = -1;

    do {
        int scanres = scanf(" %d", &choice);

        /* If the scanf fails or the choice is less than 0 or above the max index, we tell the user the 
            input wasn't recognized and try again */
        if (scanres == 0 || choice < 0 || choice > max_index) {
            choice = -1;
            on_invalid_input();
        }
        clear_input_buffer();
        
    } while(choice == -1);   

    return choice;
}

/* When input is invalid, we call this standardized print */
void on_invalid_input(void) {
    printf("Ugyldigt input, proev igen.\n");
}

/* Prints all the details of a recipe */
void print_recipe(recipe recipe, float serving_size) {
    int i;

    printf("\n%s\nTid: %d\n", recipe.name, recipe.time);

    printf("Ingredienser:\n");
    for (i = 0; i < recipe.number_of_ingredients; i++) {
        /*  Check if the floating point value is the same as the integer value,
            in which case we don't want to print any decimals */
        float amount = recipe.ingredients[i].amount * serving_size;
        int floored_amount = (int)floor(amount);

        if (floored_amount == amount)
            printf("- %d %s %s\n", 
                floored_amount, recipe.ingredients[i].unit, recipe.ingredients[i].name);
        else
            printf("- %.2f %s %s\n", 
                amount, recipe.ingredients[i].unit, recipe.ingredients[i].name);
    }

    printf("\nFremangsmaade:\n%s\n\n", recipe.procedure);
}

/* Clears the input buffer and yields until the user enters a character on the keyboard */
void press_any_key_to_continue(void) {
    printf("Tryk paa en vilkaarlig tast paa tastaturet for at gaa tilbage til menuen.\n");
    getch();    
}

/* Ask the user how many people they cook for (to scale the amount of ingredients when printing recipes) */
float get_serving_size(void) {
    int max_serving_size = 10; /* Assumes that nobody cooks for more than 10 people at once */
    int choice = 0;
    float default_serving_size = 4.0f;

    do {
        printf("Hvor mange personer laver du mad til?\n");
        
        choice = prompt_for_index_to_change(max_serving_size);
        if (choice == 0) 
            on_invalid_input();

    } while (choice == 0);
    
    return choice / default_serving_size;
}

/* Clears the screen by calling the system command cls */
void clear_screen(void) {
    system("cls");
}