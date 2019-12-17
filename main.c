/*
    Made by 
        Group: A324 | First semester project AAU
        Subject: "Den Digitale Madplan"
    
    Purpose: 
        Generation and editing of a weekplan with focus on health for a person starting their fitness journey.
        Includes shopping list, user preferences and detailed descriptions of 20+ recipes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "./utility/utility.h"
#include "./user_prefs/user_prefs.h"
#include "./database/database.h"
#include "./weekplan/weekplan.h"
#include "./shopping_list/shopping_list.h"

/* Prototype methods */
void main_menu(void);
void weekplan_menu(void);
void exit_program(void);

/* Main function that handles setup, menus and exiting */
int main(void) {
    char **file_names;

    /* Clear any text inside the window when the program starts */
    clear_screen();
    
    /* Count the number of recipes in the recipe folder and initialize the file name array
       Also gets all file names for every recipe in the database */
    file_names = get_and_count_recipe_files();
    
    /* Initialize the database */
    get_database(file_names);
    /* Runs the initial setup if the setup file is empty or does not exist */
    check_setup();

    /* Checks if a saved weekplan file already exists. If it does, it loads it. 
       Otherwise it allocates memory for the weekplan variable and makes a random weekplan for the user. */
    if (weekplan_exists())
        load_weekplan();
    else {
        weekplan = calloc(sizeof(int), DAYS_IN_WEEK);
        make_random_weekplan();

        printf("Vi har lavet en tilfaeldig ugeplan til dig.\n"
                "Du kan gaa ind i ugeplan-menuen og redigere ugeplanen.\n\n");
                
        press_any_key_to_continue();
    }

    /* Initialize the main menu */
    main_menu();

    /* Program should exit from the main menu function in case (4) so something is wrong if we get to this! */
    printf("\nEXIT FAILURE!");
    return EXIT_FAILURE;
}

/* Prints main menu for user navigation - reads input from 1-4 and calls the corresponding methods */
void main_menu(void) {
    int menu_selector = 0;
    clear_screen();

    while (menu_selector != 4) {
        int scanres = 0;

        printf("1. Ugeplan-menu\n2. Vis indkoebsliste\n3. Skift praeferencer\n4. Exit\n");
        scanres = scanf("%d", &menu_selector);

        /* Pick sub-menu from user input */
        switch(menu_selector) {
            case 1: weekplan_menu();           break;
            case 2: print_groceries();         break;
            case 3: change_user_preferences(); break;
            case 4: exit_program();
            default: clear_screen(); on_invalid_input();
        }
        menu_selector = 0;
        
        /* If the scanf failed, we clear the input buffer to avoid buffer overflow */
        if (scanres == 0)
            clear_input_buffer();
    }
}

/* Weekplan sub menu accessed via the main menu - same input principle as with the main menu */
void weekplan_menu(void) {
    int menu_selector = 0;
    clear_screen();

    while(menu_selector != 4) {
        int scanres = 0;

        printf("1. Vis nuvaerende ugeplan\n2. Generer ny ugeplan\n3. Rediger nuvaerende ugeplan \n4. Tilbage til hovedmenu\n");
        scanres = scanf("%d", &menu_selector);
        
        /* Pick sub-menu from user input */
        switch (menu_selector) {
            case 1:  print_weekplan_recipe(); break;
            case 2:  new_weekplan_prompt();   break;
            case 3:  change_weekplan();       break;
            case 4:  clear_screen();          return;
            default: clear_screen(); on_invalid_input();
        }
        menu_selector = 0;

        if (scanres == 0)
            clear_input_buffer();
    }
}

/* Frees the memory allocated for the recipe database and closes the program */
void exit_program(void) {
    free(recipe_database);
    exit(EXIT_SUCCESS);
}