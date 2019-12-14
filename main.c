#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "./utility/utility.h"
#include "./user_prefs/user_prefs.h"
#include "./database/database.h"
#include "./weekplan/weekplan.h"
#include "./shopping_list/shopping_list.h"

void exit_program(void);
void weekplan_menu(void);
void main_menu(void);
void on_invalid_input(void);
void new_weekplan_prompt(void);

int main(void) {
    /* Initialize the database */
    number_of_recipes = count_recipe_files();
    char *file_names[number_of_recipes];

    get_file_names(file_names);
    recipe_database = get_database(file_names);
    
    check_setup();

    if (weekplan_exists())
        load_weekplan();
    else {
        /* Weekplan does not exist (happens the first time the program runs), so we generate one for the user */
        weekplan = calloc(sizeof(int), DAYS_IN_WEEK);
        make_random_weekplan();

        printf("Vi har lavet en tilfaeldig ugeplan til dig.\n"
                "Du kan gaa ind i ugeplan-menuen og redigere ugeplanen.\n");
                
        press_any_key_to_continue();
    }

    main_menu();

    /* Program should exit from the main menu function (4) so something is wrong if we get to this! */
    printf("\nEXIT FAILURE");
    return EXIT_FAILURE;
}

void main_menu(void) {
    int menu_selector = 0;
    system("cls");

    while (menu_selector != 4) {
        int scanres = 0;

        printf("1. Ugeplan-menu\n2. Vis indkoebsliste\n3. Skift praeferencer\n4. Exit\n");
        scanres = scanf("%d", &menu_selector);

        switch(menu_selector) {
            case 1: weekplan_menu();           break;
            case 2: print_groceries();         break;
            case 3: change_user_preferences(); break;
            case 4: exit_program();
            default: on_invalid_input();
        }
        menu_selector = 0;

        if (scanres == 0)
            clear_input_buffer();
    }
}

void weekplan_menu(void) {
    int menu_selector = 0;
    system("cls");

    while(menu_selector != 4) {
        int scanres = 0;

        printf("1. Vis nuvaerende ugeplan\n2. Generer ny ugeplan\n3. Rediger nuvaerende ugeplan \n4. Tilbage til hovedmenu\n");
        scanres = scanf("%d", &menu_selector);
        
        switch (menu_selector) {
            case 1:  print_weekplan_recipe(); break;
            case 2:  new_weekplan_prompt();   break;
            case 3:  change_weekplan();       break;
            case 4:  main_menu();             break;
            default: on_invalid_input();
        }
        menu_selector = 0;

        if (scanres == 0)
            clear_input_buffer();
    }
}

void new_weekplan_prompt(void) {
    if (weekplan_exists()) {
        printf("Er du sikker paa, at du vil generer en ny madplan og overskrive den nuvaerende? (y/n)\n");
        
        if (yes_no_prompt())
            make_random_weekplan();
    } 
    else {
        make_random_weekplan();
        printf("Made new weekplan successfully\n");
    }

    press_any_key_to_continue();
    weekplan_menu();
}

void exit_program(void) {
    free(recipe_database);
    exit(EXIT_SUCCESS);
}